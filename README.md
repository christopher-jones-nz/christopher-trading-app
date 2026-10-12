# Distributed Real-Time Market Data & Event-Driven Analytics Engine

A high-throughput, low-latency financial telemetry and trading platform designed to ingest, process, and distribute raw market data in real time. The system leverages a multi-threaded C++ core for sub-millisecond data ingestion and order matching, coupled with Apache Spark and event streams for scalable, distributed analytics and risk evaluation.

## System Architecture

* **Low-Latency Core (C++)**: Handles raw UDP/TCP binary packets using strict memory alignment and zero-copy parsing. Coordinates data passing between network and processing threads via a custom atomic lock-free Single-Producer, Single-Consumer (SPSC) ring buffer.
* **Distributed Analytics**: Integrates Apache Spark and event-streaming architectures (such as Apache Kafka) to aggregate and analyze high-velocity trade events, enabling complex risk evaluations and historical querying on the data lake.
* **Operations UI & APIs**: Microservices exposing processed data via gRPC/FastAPI, visualized on a React/TypeScript real-time WebSocket dashboard.
* **Infrastructure & Observability**: Containerized components built for Kubernetes deployments, instrumented with OpenTelemetry, Prometheus, and Grafana for full-stack monitoring.

## Repository Structure

```text
trading-engine/
├── scripts/
│   └── mock_exchange.py      # Python script to broadcast fake UDP market data for local testing
├── src/
│   ├── core/                 # Core business logic (e.g., types.hpp, order book logic)
│   ├── network/              # UDP/TCP socket listeners and multicast handlers
│   ├── utils/                # System utilities (e.g., spsc_queue.hpp lock-free ring buffer)
│   └── main.cpp              # Application entry point wiring threads and ingestion queues
├── tests/
│   ├── bench_queue.cpp       # Benchmarking tests for lock-free queue throughput
│   └── test_orderbook.cpp    # Unit tests for order matching and state updates
├── CMakeLists.txt            # CMake build configuration for the C++ engine
└── setup.sh                  # Bash script for initial environment provisioning

```

## Prerequisites

* **C++17 Compiler or higher**: Required for `std::atomic` and modern memory alignment features.
* **CMake (3.10+)**: For building the C++ core engine.
* **Apache Spark**: For running the distributed analytics and market data processing pipelines.
* **Python 3.8+**: For executing the mock exchange data generators.
* **Linux/Ubuntu Environment**: Highly recommended for deploying the low-latency networking stack and minimizing network jitter.

## Getting Started

**1. Environment Setup**
Execute the provided setup script to install necessary system dependencies and libraries:

```bash
chmod +x setup.sh
./setup.sh

```

**2. Build the C++ Engine**
Use CMake to configure and compile the low-latency ingestion core:

```bash
mkdir build
cd build
cmake ..
make

```

**3. Start the Market Data Producer**
In a separate terminal, run the Python script to start pumping fake market data over the network:

```bash
python3 scripts/mock_exchange.py

```

**4. Execute the Core Engine**
Run the compiled C++ binary to begin listening to the UDP stream and processing the market events:

```bash
./build/trading-engine

```

## Testing and Benchmarking

The `tests` directory contains dedicated unit tests and performance benchmarks to ensure the engine meets sub-millisecond latency guarantees.

To run the order book unit tests:

```bash
./build/test_orderbook

```

To run the maximum throughput benchmark for the lock-free ring buffer:

```bash
./build/bench_queue

```
