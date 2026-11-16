#include <iostream>
#include <optional>

#include "adaptive_cache.h"
#include "lfu_cache.h"
#include "lru_cache.h"

int main() {
    std::cout << "--- Testing LRU Cache ---\n";
    LRUCache<int, int> lru(2);
    lru.put(1, 100);
    lru.put(2, 200);
    lru.get(1);
    lru.put(3, 300);
    std::cout << (lru.get(2) == std::nullopt ? "LRU Passed!\n" : "LRU Failed\n");

    std::cout << "\n--- Testing LFU Cache ---\n";
    LFUCache<int, int> lfu(2);
    lfu.put(1, 10);
    lfu.put(2, 20);
    lfu.get(1);
    lfu.put(3, 30);
    std::cout << (lfu.get(2) == std::nullopt ? "LFU Passed!\n" : "LFU Failed\n");

    std::cout << "\n--- Testing ADAPTIVE Cache ---\n";
    // Capacity 2 gives one recency slot and one frequency slot.
    AdaptiveCache<int, int> adaptive(2);
    adaptive.put(1, 100);
    adaptive.get(1);  // The second touch promotes key 1 to the frequency tier.
    adaptive.put(2, 200);
    adaptive.put(3, 300);  // The recency tier evicts key 2.

    std::cout << "Get 1: " << adaptive.get(1).value_or(-1) << "\n";
    std::cout << "Get 2: " << adaptive.get(2).value_or(-1) << "\n";
    std::cout << "Get 3: " << adaptive.get(3).value_or(-1) << "\n";
    return 0;
}
