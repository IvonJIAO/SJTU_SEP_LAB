#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

///移动构造函数
template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer { other.pointer} {
    other.pointer = nullptr;
}
//？（）和{}都可以用来初始化对象 {}可以避免一些类型转换的问题


template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete pointer; /// delete会调用析构对象
    pointer=nullptr;
}

//？移动赋值运算符
//？移动nullptr不被这个包含？
template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
    if (this != &other ) { // check self-assignment
        //？this指针不可能是nullptr
        delete pointer; //销毁他现在管理的内存
        pointer = other.pointer;
        other.pointer = nullptr;
    }
    return *this;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(std::nullptr_t) noexcept {
    delete pointer; // delete the current pointer
    pointer = nullptr; // set to nullptr
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) noexcept
{
    delete pointer;
    pointer = ptr;
}

//？ You need to return the pointer holds to the memory.
template <typename T>
T *UniquePtr<T>::release() noexcept
{
    T*temp=pointer;
    pointer = nullptr;
    return temp;
}

#endif  // UNIQUE_PTR_IMPL_H
