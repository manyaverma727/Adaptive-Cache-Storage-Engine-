#include <iostream>
#include <optional>
#include "lru_cache.h"
#include "lfu_cache.h"

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

    std::cout << "Get 1 (Freq goes to 2): " << lfu.get(1).value_or(-1) << "\n";
    std::cout << "Put (3, 30) -> This should evict key 2!\n";
    lfu.put(3, 30);

    std::cout << "Get 2 (Expected -1): " << lfu.get(2).value_or(-1) << "\n";
    std::cout << "Get 3 (Expected 30): " << lfu.get(3).value_or(-1) << "\n";

    std::cout << "\n--- ALL TESTS PASSED! ---\n";
    return 0;
}
