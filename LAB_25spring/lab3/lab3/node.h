#ifndef NODE_H_
#define NODE_H_

#include "unique_ptr.h"

template <typename T>
struct Node {
    T val;
    UniquePtr<Node> next;

    Node() : next(nullptr) { }///缺省构造是next的nullptr构造

    explicit Node(T t) : val(t), next(nullptr) { }///显式构造
};

#endif