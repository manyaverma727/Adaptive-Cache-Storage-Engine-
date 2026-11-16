# High-Performance Adaptive Cache Storage Engine

A production-grade, $O(1)$ memory-safe caching engine built entirely from scratch in C++17. This engine implements Least Recently Used (LRU), Least Frequently Used (LFU), and a custom 2-Queue (2Q) Adaptive Controller to dynamically balance memory between recency and frequency.

## 🚀 Technical Highlights
* **Strict $O(1)$ Time Complexity:** All operations (insert, fetch, update, and evict) execute in constant time, bypassing the $O(N)$ bottlenecks of standard C++ arrays and lists.
* **Custom Memory Layer:** Engineered a raw Doubly Linked List utilizing **Sentinel Nodes** (dummy head/tail) to guarantee safe, branchless pointer manipulation and eliminate segmentation faults.
* **Header-Only Template Architecture:** Built using C++ templates (`<Key, Value>`) across `.h` files for clean, decoupled code that can instantly type-cast to any data type without linking overhead.

## 🧠 Architecture Overview
The engine is decoupled into three progressive layers:

1. **The Memory Layer (`doubly_linked_list.h`):** The foundational data structure ensuring $O(1)$ pointer rewiring for instant data promotion and eviction.
2. **The Core Engines (`lru_cache.h`, `lfu_cache.h`):**
   * **LRU:** Combines the custom linked list with `std::unordered_map` to handle "bursty" recency-based traffic.
   * **LFU:** Utilizes a nested Hash Map structure mapping frequency counts to dedicated Linked Lists for "viral" popularity-based traffic.
3. **The Adaptive Controller (`adaptive_cache.h`):** Inspired by enterprise databases like PostgreSQL. It splits capacity into two queues (L1 Recency and L2 Frequency). Items accessed once enter L1; items accessed a second time are instantly promoted to L2, dynamically balancing cache capacity based on live user behavior.

## 🛠️ Build and Run
This project uses a zero-dependency header-only structure. You can compile and run the benchmark suite using standard `clang++` or `g++`:

```bash
# Clone the repository
git clone [https://github.com/manyaverma727/Adaptive-Cache-Storage-Engine-.git](https://github.com/manyaverma727/Adaptive-Cache-Storage-Engine-.git)
cd Adaptive-Cache-Storage-Engine-

# Compile using C++17
clang++ -std=c++17 -Iinclude src/main.cpp -o cache_engine

# Run the engine
./cache_engine