#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <atomic>
#include <iostream>
#include "../core/types.hpp"
#include "../utils/spsc_queue.hpp"

template <size_t QueueSize>
class UDPReceiver
{
private:
  int sockfd{-1};
  int port;

public:
  explicit UDPReceiver(int port) : port(port) {}

  ~UDPReceiver()
  {
    if (sockfd >= 0)
    {
      ::close(sockfd);
    }
  }

  bool init()
  {
    sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
      return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0)
    {
      return false;
    }
    return true;
  }

  void start_listening(LockFreeSPSCQueue<MarketUpdate, QueueSize> &queue, std::atomic<bool> &running)
  {
    MarketUpdate update;

    while (running.load(std::memory_order_relaxed))
    {
      // Non-blocking socket receive directly into struct address
      ssize_t bytes_read = ::recv(sockfd, &update, sizeof(MarketUpdate), MSG_DONTWAIT);

      if (bytes_read == sizeof(MarketUpdate))
      {
        // Spin until queue accepts item, injecting a hardware pause to reduce bus contention
        while (!queue.push(update) && running.load(std::memory_order_relaxed))
        {
#if defined(__x86_64__) || defined(_M_X64)
          asm volatile("pause" ::: "memory");
#endif
        }
      }
    }
  }
};