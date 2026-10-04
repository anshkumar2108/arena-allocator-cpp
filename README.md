# arena-allocator-cpp

## Project Overview

A custom arena and fixed-size pool allocator with O(1) alloc/dealloc time and proper CPU alignment, wrapped in an STL-compatible interface.

The project is built in three layers:

- **Layer 1 — Arena** (`include/Arena.hpp`): A raw memory region acquired once at program start. All allocations come from this region via a bump pointer. No `malloc` calls after initialization.
- **Layer 2 — PoolAllocator** (`include/PoolAllocator.hpp`): Carves the arena into fixed-size blocks using an intrusive free list. O(1) allocation and deallocation, slot reuse verified.
- **Layer 3 — Allocator** (`include/Allocator.hpp`): An STL-compatible wrapper implementing the `allocator_traits` interface with rebind support. Works with standard containers such as `std::list`.

> Single-threaded only. Concurrent or multi-threaded use is not supported.

---

## Why I Built This

About two months ago I built my own single-threaded `malloc`/`calloc` allocator in C using a free list with splitting, coalescing, and `realloc` support. When I benchmarked it against the system `malloc`, my allocator took around 20 seconds for the same operations that the default finished in milliseconds.

That result pushed me to understand how allocators actually work at a lower level. I learned about arena and fixed-size pool allocators — where after one large upfront allocation, every subsequent alloc/dealloc is just pointer arithmetic. So I built this project to put that into practice.

---

## Build Instructions

Requirements: `g++` with C++17 support, `cmake` 3.10+

```bash
chmod +x run.sh
./run.sh
```

This will configure the project with CMake, build all targets, run all three tests under AddressSanitizer and UBSanitizer, and run the benchmark.

---

## Benchmark Results

Tested on WSL2 (Ubuntu, GCC 15.2.0), compiled with `-O2`.

Operation: 10,000 `push_back` insertions into `std::list<int>` with a warmup run before measurement.

| Allocator | Time (microseconds) |
|---|---|
| Custom PoolAllocator | ~120 |
| Default std::allocator | ~300 |

**~2.5x faster** than the default allocator.

The default `std::allocator` calls `malloc` per node — thread-safe with locks and bookkeeping overhead. The custom allocator bypasses all of that after the initial arena allocation.

---

## Concepts Covered

- **Move Semantics** — move constructor, move assignment, `noexcept`, `std::move`
- **Rule of Five** — explicit `= delete` on copy ops for exclusive ownership
- **Object Lifetime** — RAII, scope-based destruction, dangling reference prevention
- **Placement New** — manual construction/destruction on raw memory, `alignas`
- **STL Allocator Interface** — `allocator_traits`, `rebind`, `value_type`, `allocate`, `deallocate`

All tests compiled and verified with `-fsanitize=address,undefined`.
