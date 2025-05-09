#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>
#include <iostream>
#include "node.h"

template <typename T>
void Stack<T>::push(T t) {
    //std::cout<<"before push size: "<<sz<< std::endl;
    UniquePtr<Node<T>> tmp = std::move(head); // Save current head
    /*assert(tmp != nullptr);*/
    head = MakeUnique<Node<T>>(t);
    head->next = std::move(tmp);
    sz++;
    //std::cout<<"current value: "<<head->val<<std::endl;
    /*assert(head != nullptr); // After push, head must not be nullptr
    assert(head->next != nullptr);//！head->next指向了空！！*/
    assert(sz > 0);
    //std::cout << "Stack size after push: " << sz << std::endl;
    //std::cout << "Debug: head=" << (head == nullptr) << ", sz=" << sz << std::endl;
}

template <typename T>
void Stack<T>::pop() {
    //assert(head->next != nullptr);
    assert(head != nullptr);
    //std::cout << "before pop size "<<sz<< std::endl;
    if (empty()) {
        std::cerr << "Error: Attempt to pop from an empty stack." << std::endl;
        return;
    }

    //std::cout<<head->val << std::endl;//！ 为什么编译不过
    //assert(head->next != nullptr);
    //assert(head != nullptr);
    //std::cout << head->next->val << std::endl;
    //std::cout << "after check all well "<< std::endl;

    //std::cout << "Debug: head=" << (head == nullptr) << ", sz=" << sz << std::endl;
    if (sz==1) {
        head=nullptr;
        sz--;
    }
    //！最后只剩下一个元素的时候 head指向nullptr要调用单独的函数

    else
    {
        head = std::move(head->next);//！每次pop到最后一层的时候，head->next确实是空的
        //std::cout << (head->next==nullptr) << std::endl;
        //assert(head != nullptr);//！head没问题
        //std::cout << head->val<<std::endl;
        //assert(head->next != nullptr);
        //这里next指向了空！！


        // Move to next node, old node auto-deleted
        //std::cout << "Debug: head=" << (head == nullptr) << ", sz=" << sz << std::endl;
        sz--;
        //std::cout << "Stack size after pop: " << sz << std::endl;

    }
}



template <typename T>
T &Stack<T>::top() {
    assert(sz > 0); // Ensure there is an element on top
    assert(head != nullptr); // Head must not be nullptr
    //assert(head->next != nullptr);
    if (empty()) {
        throw std::out_of_range("Error: Attempt to access top of an empty stack.");
    }
    //std::cout<<"before top size: "<<sz<<std::endl;
    return head->val;
}

template <typename T>
bool Stack<T>::empty() const {

    assert((head == nullptr) == (sz == 0)); // Head is nullptr iff size == 0
    return sz == 0;
}

template <typename T>
size_t Stack<T>::size() const {
    return sz;
}

#endif  // STACK_IMPL_H
