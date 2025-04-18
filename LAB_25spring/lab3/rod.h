#ifndef ROD_H
#define ROD_H

#include "canvas.h"
#include "disk.h"
#include "stack.h"
#include <string>
#include <cstddef>

class Rod {
    int capacity;
    int id;

public:
    Stack<const Disk> stack;

    Rod(int capacity, int id);
    bool push(Disk d);
    void pop();
    [[nodiscard]] const Disk &top();
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool full() const;
    void draw(Canvas &canvas);
    std::string toString();
};

#endif
