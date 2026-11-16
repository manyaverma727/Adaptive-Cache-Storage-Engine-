#include <iostream>
#include "lru_cache.h" // <-- THIS IS THE CRUCIAL LINE

int main() {
    std::cout << "--- Starting LRU Cache Test ---\n";
    LRUCache<int, int> lru(2);
    lru.put(1, 100);
    std::cout << "Put (1, 100)\n";
    lru.put(2, 200);
    std::cout << "Put (2, 200)\n";
    std::cout << "Get 1: " << lru.get(1).value_or(-1) << " (Expected 100)\n";
    lru.put(3, 300);
    std::cout << "Put (3, 300) -> This should evict key 2!\n";
    std::cout << "Get 2: " << lru.get(2).value_or(-1) << " (Expected -1, Cache Miss)\n";
    std::cout << "Get 3: " << lru.get(3).value_or(-1) << " (Expected 300)\n";
    std::cout << "--- Test Passed! ---\n";
    return 0;
}