#!/bin/bash
# setup.sh - Provisions the Ubuntu development environment

# Exit immediately if a command exits with a non-zero status
set -e

echo "Starting environment setup for Distributed Trading Engine..."

# Update package lists
echo "Updating apt repositories..."
sudo apt-get update -y

# Install core build tools (C++ compiler, Make, CMake, Debugger)
echo "Installing build-essential, cmake, and gdb..."
sudo apt-get install -y build-essential cmake gdb git

# Install networking tools for debugging UDP/TCP traffic
echo "Installing net-tools and tcpdump..."
sudo apt-get install -y net-tools tcpdump python3

# Install testing frameworks (Google Test and Google Benchmark)
echo "Installing Google Test and Google Benchmark libraries..."
sudo apt-get install -y libgtest-dev libbenchmark-dev

echo "Environment setup complete!"
echo "To build the project, run:"
echo "mkdir build && cd build && cmake .. && make"