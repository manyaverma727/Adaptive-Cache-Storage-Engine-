#pragma once
#include <cstddef>
#include <unordered_map>
#include <optional>
#include "cache_node.h"
#include "doubly_linked_list.h"

template <typename Key, typename Value>
class LRUCache {
private:
    std::size_t capacity;
    DoublyLinkedList<Key, Value> dll;
    std::unordered_map<Key, CacheNode<Key, Value>*> cache_map;

public:
    explicit LRUCache(std::size_t cap) : capacity(cap) {}

    ~LRUCache() {
        for (auto& entry : cache_map) {
            delete entry.second;
        }
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;

    std::optional<Value> get(const Key& key) {
        const auto it = cache_map.find(key);
        if (it == cache_map.end()) {
            return std::nullopt;
        }
        CacheNode<Key, Value>* node = it->second;
        dll.removeNode(node);
        dll.insertAtFront(node);
        return node->value;
    }

    void put(Key key, Value value) {
        if (capacity == 0) {
            return;
        }

        const auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            CacheNode<Key, Value>* node = it->second;
            node->value = value;
            dll.removeNode(node);
            dll.insertAtFront(node);
        } else {
            if (cache_map.size() >= capacity) {
                CacheNode<Key, Value>* tail = dll.removeTail();
                if (tail) {
                    cache_map.erase(tail->key);
                    delete tail;
                }
            }
            CacheNode<Key, Value>* new_node = new CacheNode<Key, Value>(key, value);
            dll.insertAtFront(new_node);
            cache_map[key] = new_node;
        }
    }
};
