#ifndef BOARD_H
#define BOARD_H

#include "disk.h"
#include "rod.h"
#include "stack.h"
#include <unordered_set> //用于查找是否满足递归路径


constexpr static size_t ROD_SIZE = 3;

class Board {
    const int num_disk;
    Rod rods[ROD_SIZE];
    Disk *disks;///用指针开一个动态数组
    Stack<std::pair<int, int>> history;
    ///用于记录手动移动的历史
    std::unordered_set<std::string> optimal_states;


public:
    explicit Board(int num_disk);
    //？explicit代表不能隐式调用构造函数

    ~Board();
    void draw();
    void move(int from, int to, bool log = true);
    [[nodiscard]] bool win() const;
    //？若没有使用这个函数的返回值 nodiscard会报错

    void autoplay();
    void hint();
    std::string toString();
};

#endif
