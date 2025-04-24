#ifndef DISK_H
#define DISK_H

#include "canvas.h"

class Disk {
public:
    int id;
    int val;    ///圆盘的大小


    Disk() : id(-1), val(-1) {} ///缺省构造函数
    Disk(const int id, const int val) : id(id), val(val) { }

    //？在什么时候调用draw？ board中调用rod的draw，rod中调用disk的draw
    ///画盘子的时候要指定层数和位置 loc从0开始计数
    void draw(Canvas &canvas, const int level, const int rod_loc) const {
        const int s_x = 5 + (rod_loc * 15) - (val / 2); ///减去val的一半，才能画出正确的位置
        const int y = Canvas::HEIGHT - (level + 1) * 2;
        for (int i = 0; i < val; i++)
        {
            canvas.buffer[y][s_x + i] = '*';
        }
    }
};

#endif
