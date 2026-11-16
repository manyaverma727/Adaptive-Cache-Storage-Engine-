#pragma once

#include "cache_node.h"

template <typename Key, typename Value>
class DoublyLinkedList {
private:
    CacheNode<Key, Value>* head = nullptr;
    CacheNode<Key, Value>* tail = nullptr;

public:
    bool isEmpty() const {
        return head == nullptr;
    }

    void insertAtFront(CacheNode<Key, Value>* node) {
        if (node == nullptr) {
            return;
        }

        node->prev = nullptr;
        node->next = head;

        if (head != nullptr) {
            head->prev = node;
        } else {
            tail = node;
        }

        head = node;
    }

    void removeNode(CacheNode<Key, Value>* node) {
        if (node == nullptr) {
            return;
        }

        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else if (head == node) {
            head = node->next;
        } else {
            return;
        }

        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }

        node->prev = nullptr;
        node->next = nullptr;
    }

    CacheNode<Key, Value>* removeTail() {
        CacheNode<Key, Value>* node = tail;
        if (node != nullptr) {
            removeNode(node);
        }
        return node;
    }
};
