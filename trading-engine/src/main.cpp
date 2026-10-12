#include "spsc_queue.hpp"
#include "types.hpp"

// Instantiate market queue
LockFreeSPSCQueue<MarketUpdate> marketQueue;

// Write thread loop functions

int main() {
    // Example usage of the marketQueue
    MarketUpdate update;
    // Populate update with data...

    // Enqueue the update
    if (marketQueue.enqueue(update)) {
        // Successfully enqueued
    } else {
        // Queue is full, handle accordingly
    }

    // Dequeue in another thread
    MarketUpdate dequeuedUpdate;
    if (marketQueue.dequeue(dequeuedUpdate)) {
        // Successfully dequeued, process the update
    } else {
        // Queue is empty, handle accordingly
    }

    return 0;
}