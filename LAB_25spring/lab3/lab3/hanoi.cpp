#include <climits>
#include <iostream>
#include <string>

#include "board.h"

using namespace std;

int main() {
    while (true) {

        ///第一部分
        //*第一个输入只能是数字判断输入是否合法如果合法，创建一个Board对象
        //*如果输入不合法，提示用户重新输入

        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }
//TODO 异常处理
        if (input< "1" || input > "5") {
            cout << "Invalid input. Please enter a number between 1 and 5." << endl;
            continue;
        }
        int num_disk = stoi(input);
        Board board(num_disk);
        board.draw();



        ///第二部分
        //*打印汉诺塔的状态，
        //？在board::draw中调用canvas函数
        //*随后打印 Move a disk. Format: x y (or 'H' for a hint) ，
        //*检查输入是否合法
        while (true) {
            cout << "Move a disk. Format: x y (or 'H' for a hint)" << endl;
            string input;
            getline(cin, input);

            if (input == "Q") {
                break;
            }
            if (input == "H") {
                board.hint(); /// 我的hint函数里已经包含draw了
                continue;
            }

            // 检查输入是否为两个数字
            int x = -1, y = -1;
            bool valid = false;

            if (input.size() >= 3 && isdigit(input[0]) && input[1] == ' ' && isdigit(input[2]))
            {
                x = input[0] - '0';
                y = input[2] - '0';
                if (x >= 0 && x <= 2 && y >= 0 && y <= 2 && x != y) {
                    valid = true;
                }
            }

            if (valid) {
                board.move(x, y, false); ///移动是否符合规则再move函数中实现
                board.draw(); // 打印当前状态
            } else {
                std::cout << "Invalid input. Please enter two distinct numbers between 0 and 2." << std::endl;
            }
        }

        ///第三部分
        //*如果输入是0 0自动移动 如果是H 提示是否在递归路径上
        //？比较使用字符串进行


    }
    return 0;
}
