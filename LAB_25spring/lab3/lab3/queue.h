#ifndef QUEUE_H
#define QUEUE_H

// Read-only definition
// NOTE: DON'T modify this file

#include "node.h"
#include <cstddef>

template <typename T>
class Queue {
    int sz;
    UniquePtr<Node<T>> head;

    // Question: why we use raw pointer for tail?
    Node<T> * tail;
    //？ 每个Node有且仅有一个Uniqueptr，但我们需要记录tail的地址以便于插入
    //？ 所以我们使用原始指针来记录tail的地址

public:
    Queue() : sz(0), head(nullptr), tail(nullptr) { }

    void push(T t);
    void pop();
    T &front();
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
};

#include "queue_impl.h"
#endif
