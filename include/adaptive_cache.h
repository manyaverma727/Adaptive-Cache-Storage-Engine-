#pragma once

#include <cstddef>
#include <map>
#include <optional>
#include <unordered_map>

#include "cache_node.h"
#include "doubly_linked_list.h"

// New items start in the recency tier. A second access promotes them to the
// frequency tier, where least-frequently-used entries are evicted first.
template <typename Key, typename Value>
class AdaptiveCache {
private:
    using Node = CacheNode<Key, Value>;
    using List = DoublyLinkedList<Key, Value>;

    std::size_t l1_capacity_;
    std::size_t l2_capacity_;

    std::unordered_map<Key, Node*> l1_items_;
    std::unordered_map<Key, Node*> l2_items_;
    List l1_recency_;
    std::map<int, List*> l2_frequencies_;

    List& listForFrequency(int frequency) {
        auto it = l2_frequencies_.find(frequency);
        if (it == l2_frequencies_.end()) {
            it = l2_frequencies_.emplace(frequency, new List()).first;
        }
        return *it->second;
    }

    void removeFromL2(Node* node) {
        const auto bucket = l2_frequencies_.find(node->frequency);
        if (bucket == l2_frequencies_.end()) {
            return;
        }

        bucket->second->removeNode(node);
        if (bucket->second->isEmpty()) {
            delete bucket->second;
            l2_frequencies_.erase(bucket);
        }
    }

    void touchL2(Node* node) {
        removeFromL2(node);
        ++node->frequency;
        listForFrequency(node->frequency).insertAtFront(node);
    }

    void evictL2IfFull() {
        if (l2_items_.size() < l2_capacity_ || l2_frequencies_.empty()) {
            return;
        }

        auto bucket = l2_frequencies_.begin();
        Node* victim = bucket->second->removeTail();
        if (victim == nullptr) {
            return;
        }

        l2_items_.erase(victim->key);
        if (bucket->second->isEmpty()) {
            delete bucket->second;
            l2_frequencies_.erase(bucket);
        }
        delete victim;
    }

    void promoteToL2(Node* node) {
        l1_recency_.removeNode(node);
        l1_items_.erase(node->key);

        evictL2IfFull();
        ++node->frequency;
        listForFrequency(node->frequency).insertAtFront(node);
        l2_items_[node->key] = node;
    }

public:
    explicit AdaptiveCache(std::size_t capacity)
        : l1_capacity_(capacity < 2 ? capacity : capacity / 2),
          l2_capacity_(capacity - l1_capacity_) {}

    ~AdaptiveCache() {
        for (const auto& item : l1_items_) {
            delete item.second;
        }
        for (const auto& item : l2_items_) {
            delete item.second;
        }
        for (const auto& bucket : l2_frequencies_) {
            delete bucket.second;
        }
    }

    AdaptiveCache(const AdaptiveCache&) = delete;
    AdaptiveCache& operator=(const AdaptiveCache&) = delete;

    std::optional<Value> get(const Key& key) {
        const auto l2_item = l2_items_.find(key);
        if (l2_item != l2_items_.end()) {
            Node* node = l2_item->second;
            touchL2(node);
            return node->value;
        }

        const auto l1_item = l1_items_.find(key);
        if (l1_item == l1_items_.end()) {
            return std::nullopt;
        }

        Node* node = l1_item->second;
        if (l2_capacity_ == 0) {
            l1_recency_.removeNode(node);
            l1_recency_.insertAtFront(node);
        } else {
            promoteToL2(node);
        }
        return node->value;
    }

    void put(Key key, Value value) {
        const auto l2_item = l2_items_.find(key);
        if (l2_item != l2_items_.end()) {
            Node* node = l2_item->second;
            node->value = value;
            touchL2(node);
            return;
        }

        const auto l1_item = l1_items_.find(key);
        if (l1_item != l1_items_.end()) {
            Node* node = l1_item->second;
            node->value = value;
            if (l2_capacity_ == 0) {
                l1_recency_.removeNode(node);
                l1_recency_.insertAtFront(node);
            } else {
                promoteToL2(node);
            }
            return;
        }

        if (l1_capacity_ == 0) {
            return;
        }

        if (l1_items_.size() >= l1_capacity_) {
            Node* victim = l1_recency_.removeTail();
            if (victim != nullptr) {
                l1_items_.erase(victim->key);
                delete victim;
            }
        }

        Node* node = new Node(key, value);
        l1_recency_.insertAtFront(node);
        l1_items_[key] = node;
    }
};
