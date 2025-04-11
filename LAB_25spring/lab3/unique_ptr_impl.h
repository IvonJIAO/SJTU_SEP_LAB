#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer { /* TODO */ } { }

template <typename T>
UniquePtr<T>::~UniquePtr() {
    // TODO
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
    // TODO
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(std::nullptr_t) noexcept {
    // TODO
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) noexcept {
    // TODO
}

template <typename T>
T *UniquePtr<T>::release() noexcept {
    // TODO
}

#endif  // UNIQUE_PTR_IMPL_H
