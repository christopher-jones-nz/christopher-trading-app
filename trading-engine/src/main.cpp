#include <iostream>
#include <thread>
#include <atomic>
#include <csignal>
#include "core/types.hpp"
#include "utils/spsc_queue.hpp"
#include "network/udp_receiver.hpp"

std::atomic<bool> g_running{true};

void signal_handler(int signal) {
    if (signal == SIGINT) {
        g_running.store(false, std::memory_order_relaxed);
    }
}

int main() {
    std::signal(SIGINT, signal_handler);

    constexpr size_t QUEUE_SIZE = 1024;
    LockFreeSPSCQueue<MarketUpdate, QUEUE_SIZE> market_queue;

    UDPReceiver<QUEUE_SIZE> receiver(8080);
    if (!receiver.init()) {
        std::cerr << "Failed to bind UDP socket on port 8080\n";
        return 1;
    }

    std::cout << "Trading engine started. Listening on UDP port 8080...\n";

    // Ingestion Thread (Producer)
    std::thread network_thread([&]() {
        receiver.start_listening(market_queue, g_running);
    });

    // Processing Thread (Consumer)
    std::thread processing_thread([&]() {
        MarketUpdate update;
        uint64_t count = 0;

        while (g_running.load(std::memory_order_relaxed)) {
            if (market_queue.pop(update)) {
                count++;
                if (count % 100000 == 0) {
                    std::cout << "[Core Engine] Processed " << count 
                              << " updates. Last: " << update.symbol 
                              << " @ $" << update.price << "\n";
                }
            }
        }
    });

    network_thread.join();
    processing_thread.join();

    std::cout << "Engine shut down cleanly.\n";
    return 0;
}