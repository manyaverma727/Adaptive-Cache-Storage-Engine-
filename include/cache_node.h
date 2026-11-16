#pragma once
#include <cstdint>

template <typename Key, typename Value>
struct CacheNode {
    Key key;
    Value value;
    int frequency;
    uint64_t last_accessed;

    CacheNode* prev;
    CacheNode* next;

    CacheNode(Key k, Value v) 
        : key(k), value(v), frequency(1), last_accessed(0), prev(nullptr), next(nullptr) {}
};
