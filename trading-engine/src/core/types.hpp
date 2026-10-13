// Hold the packed MarketUpdate structs in a lock-free single-producer single-consumer queue. This allows the producer to enqueue MarketUpdate structs while the consumer dequeues them without the need for locks, ensuring low latency and high throughput in a multi-threaded environment.
#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct MarketUpdate {
    uint64_t timestamp; // Epoch nanoseconds
    char symbol[8];    // Ticker (e.g., "AAPL\0\0\0\0")
    double price;     // Execution/Quote Price
    uint32_t size;    // Share quantity
    uint8_t is_trade;       // 1 = Trade, 0 = Order Book Update
};
#pragma pack(pop)
