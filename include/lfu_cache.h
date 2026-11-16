#pragma once
#include <cstddef>
#include <unordered_map>
#include <optional>
#include "cache_node.h"
#include "doubly_linked_list.h"

template <typename Key, typename Value>
class LFUCache {
private:
    std::size_t capacity;
    int min_freq;
    std::unordered_map<Key, CacheNode<Key, Value>*> cache_map;
    std::unordered_map<int, DoublyLinkedList<Key, Value>*> freq_map;

    void updateFrequency(CacheNode<Key, Value>* node) {
        int old_freq = node->frequency;
        freq_map[old_freq]->removeNode(node);

        // If the old frequency list is empty and it was the minimum, increment min_freq
        if (old_freq == min_freq && freq_map[old_freq]->isEmpty()) {
            min_freq++;
        }

        node->frequency++;
        int new_freq = node->frequency;

        // Create a new list for the new frequency if it doesn't exist
        if (freq_map.find(new_freq) == freq_map.end()) {
            freq_map[new_freq] = new DoublyLinkedList<Key, Value>();
        }
        freq_map[new_freq]->insertAtFront(node);
    }

public:
    explicit LFUCache(std::size_t cap) : capacity(cap), min_freq(0) {}

    ~LFUCache() {
        for (auto& entry : cache_map) {
            delete entry.second;
        }
        for (auto& pair : freq_map) {
            delete pair.second;
        }
    }

    LFUCache(const LFUCache&) = delete;
    LFUCache& operator=(const LFUCache&) = delete;

    std::optional<Value> get(const Key& key) {
        const auto it = cache_map.find(key);
        if (capacity == 0 || it == cache_map.end()) {
            return std::nullopt;
        }
        CacheNode<Key, Value>* node = it->second;
        updateFrequency(node);
        return node->value;
    }

    void put(Key key, Value value) {
        if (capacity == 0) {
            return;
        }

        const auto existing = cache_map.find(key);
        if (existing != cache_map.end()) {
            CacheNode<Key, Value>* node = existing->second;
            node->value = value;
            updateFrequency(node);
            return;
        }

        if (cache_map.size() >= capacity) {
            // Evict the Least Frequently Used item
            DoublyLinkedList<Key, Value>* min_list = freq_map[min_freq];
            CacheNode<Key, Value>* tail = min_list->removeTail();
            if (tail) {
                cache_map.erase(tail->key);
                delete tail;
            }
        }

        CacheNode<Key, Value>* new_node = new CacheNode<Key, Value>(key, value);
        cache_map[key] = new_node;

        min_freq = 1;

        if (freq_map.find(1) == freq_map.end()) {
            freq_map[1] = new DoublyLinkedList<Key, Value>();
        }
        freq_map[1]->insertAtFront(new_node);
    }
};
