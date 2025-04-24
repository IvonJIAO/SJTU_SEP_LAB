#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>

/*template <typename T>
void Stack<T>::push(T t) {
    UniquePtr<Node<T>> temp=head;
    head= UniquePtr<Node<T>>(new Node<T>(t));
    head->next=temp;
}
*/
///作为链表实现的栈 next应该指向原来的头节点

template <typename T>
void Stack<T>::push(T t) {
    UniquePtr<Node<T>> tmp = std::move(head); // 保存当前头节点
    head = UniquePtr<Node<T>>(new Node<T>(t)); // 创建新节点并将其设为头节点
    head->next = std::move(tmp); // 将原头节点链接到新头节点的 next
    sz++; // 增加栈的大小
}


template <typename T>
void Stack<T>::pop() {
    if (empty()) { return; }
    head = std::move(head->next);
    sz--;
    // assert(sz > 0);  // Ensure stack is not empty before popping
    // head = std::move(head->next);
    // sz--;
}

template <typename T>
T &Stack<T>::top() {
    if (empty()) { throw std::out_of_range("Stack is empty"); }
    return head->val;
}

template <typename T>
bool Stack<T>::empty() const {
    if (sz == 0) { return true; }
    else { return false; }
}

template <typename T>
size_t Stack<T>::size() const {
    return sz;
}

#endif  // STACK_IMPL_H
