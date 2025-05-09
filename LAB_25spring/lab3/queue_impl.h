#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

template <typename T>
void Queue<T>::push(T t) {
    if (empty()) {
        head = MakeUnique<Node<T>>(t);
        tail = head.get();  // tail指向头节点
        assert(head != nullptr && tail != nullptr);
    } else {
        assert(tail != nullptr);
        tail->next = UniquePtr<Node<T>>(new Node<T>(t));   ///新的节点应指向null
        tail = tail->next.get();  ///uniqueptr中的 .get（）函数返回的是普通指针
        assert(tail != nullptr && tail->next == nullptr); // 确保新节点被正确添加且指向null
    }
    //std::cout<<"pushed"<<std::endl;
    sz++; //！这没发现啊？！？！？！？！？！？！？！？！？！？！？！
}

//？ std::move让参数变成右值
template <typename T>
void Queue<T>::pop() {
    if (empty()) { return; }
    // 将当前头节点保存到一个临时变量
    UniquePtr<Node<T>> temp = std::move(head);

    head = std::move(temp->next);

    // 如果头节点为空，说明队列已空，更新尾指针为 nullptr
    if (!head) {
        tail = nullptr;
    }
    sz--;
}

template <typename T>
T &Queue<T>::front() {
    if (!empty()) {
        return head->val;
    }
    else {
        throw std::out_of_range("Queue is empty");
    }
}

template <typename T>
bool Queue<T>::empty() const {
    if (sz==0) {
        return true;
    }
    else
        return false;
}

template <typename T>
size_t Queue<T>::size() const {
    return sz;
}
#endif  // QUEUE_IMPL_H
