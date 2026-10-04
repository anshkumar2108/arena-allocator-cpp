#!/bin/bash
set -e

echo "Building project..."
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make

echo ""
echo "Running tests..."
echo "--- test_arena ---"
./test_arena

echo "--- test_pool ---"
./test_pool

echo "--- test_allocator ---"
./test_allocator

echo ""
echo "--- benchmark ---"
./benchmark