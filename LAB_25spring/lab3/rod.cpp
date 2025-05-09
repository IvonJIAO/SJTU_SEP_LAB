#include "rod.h"
//用来检测的
#include <iostream>
#include <cstddef>

Rod::Rod(const int capacity, const int id) : capacity(capacity), id(id) {
    //std::cout<<"Rod constructor called."<<std::endl;
}

bool Rod::push(const Disk d) {
    if (stack.size()<capacity)
        {
        stack.push(d);
        //std::cout<<"Pushed disk "<<d.val<<" to rod #"<<id<<std::endl;
        return true;
    }
    else
    {
        std::cout<<"Rod is full"<<std::endl;
        return false;
    }
}

const Disk & Rod::top()
{
    if (stack.empty())
    {
        throw std::runtime_error("Rod is empty");
        //？其实应该不会有 该逻辑应该在代码中
    }
    else
    {
        //std::cout<<"Top disk is "<<stack.top().val<<std::endl;
        return stack.top();
        /// 返回的是引用
    }
}

void Rod::pop()
{
    if (stack.empty())
    {
        std::cout<<"Stack is empty";
        //？其实应该不会有 该逻辑应该在代码中
    }
    else
    {
        stack.pop();
   /*     std::cout<<"Popped disk "//！bug one（程序一直运行到了这一步）
        <<stack.top().val<<std::endl;//！这里pop完之后top就输出不了了
        */
    }
}

size_t Rod::size() const {
    return stack.size();
}

bool Rod::empty() const {
    return stack.empty();
}
bool Rod::full() const {
    return stack.size() == capacity;
}


//！画盘子的时候不能破坏栈里的关系 用一个temp接着
///（TODO 还需要穿插一个输出“|” 的 canvas里初始化都是“ ”）这一步应该在board里做）
//？为了让toString能够准确判断 我们需要将柱子都加进去
void Rod::draw(Canvas &canvas)
{
    ///根据柱子的id来把空柱子画出来
    //std::cout<<"Drawing rod #"<<id<<"."<<std::endl;
    switch (id) {
        case 0: {
            for (int i=0;i<11;i++)
                canvas.buffer[Canvas::HEIGHT-1][i] = '-';
            for (int j=0;j<Canvas::HEIGHT;j++)
                canvas.buffer[j][5] = '|';
        break;
        }
        case 1: {
            for (int i=11;i<31;i++)
                canvas.buffer[Canvas::HEIGHT-1][i] = '-';
            for (int j=0;j<Canvas::HEIGHT;j++)
                canvas.buffer[j][20]='|';
            break;
        }
            case 2: {
            for (int i=31;i<=41;i++)
                canvas.buffer[Canvas::HEIGHT-1][i] = '-';
            for (int j=0;j<Canvas::HEIGHT;j++)
                canvas.buffer[j][35]='|';
            break;
        }
    }


/*std::cout <<"Congratulations! successfully fixed bug one:"<< std::endl;  //！通过加载断点 发现pop只能成功一次 pop有问题
//std::cout<<std::endl;
    //std::cout<<std::endl;
    */



    /// 使用临时栈保存盘子
    Stack<const Disk> tempStack;

    while (!Rod::empty()) {
        tempStack.push(std::move(Rod::top()));
        Rod::pop();
    }

    // 从临时栈绘制盘子并恢复到原栈
    while (!tempStack.empty()) {
        const Disk &d = tempStack.top();
        d.draw(canvas, Rod::size(), id); // 层数是当前栈的高度
        stack.push(std::move(d)); // 恢复到原栈
        tempStack.pop();
    }
    /*std::cout<<"Drawing rod #after"<<id<<"."<<std::endl;
    canvas.draw();*/

   /* std::cout<<"congra,fixed bug two"<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    */
}


//！不传入额外参数 需要借助已经实现的 带有canvas参数的函数来实现
std::string Rod::toString() {
    //？使用字符串的加减 从上到下把柱子上的弄进去
    std::string result;
    Canvas tempcanvas;
    tempcanvas.reset();
    draw(tempcanvas);
    ///根据不同的id 将柱子上的盘子加进去
    switch (id)
    {
        case 0:
            for (int i=0; i < Canvas::HEIGHT; i++)
                for (int j = 0; j < 11; j++)
                    result += tempcanvas.buffer[i][j];
        case 1:
            for (int i=0; i < Canvas::HEIGHT; i++)
                for (int j = 11; j < 30; j++)
                    result += tempcanvas.buffer[i][j];
        case 2:
            for (int i=0; i < Canvas::HEIGHT; i++)
                for (int j = 30; j < 41 ; j++)
                    result += tempcanvas.buffer[i][j];
    }
    return result;
}