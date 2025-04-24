#include <iostream>

#include "board.h"
#include "queue.h"

//！注意一下数组开的都是刚好大小的，编号均从0开始

Board::Board(const int num_disk)
    : num_disk(num_disk), disks(new Disk[num_disk]),
      rods{Rod(num_disk, 0), Rod(num_disk, 1), Rod(num_disk, 2)} {
    for (int i = 0; i < num_disk; i++)
    {
        disks[i] = Disk(i, num_disk +2* i);
        rods[0].push(disks[i]);///所有盘子初始化到第0根杆上
    }

    //TODO std::unordered_set<std::string> optimal_states？
}

///析构应该只要把动态数组消了？
///delete[] 能够正确地释放数组的所有元素，并调用每个元素的析构函数（如果有
Board::~Board() {
    delete [] disks;
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    for (int i = 0; i < ROD_SIZE; i++)
    {
        rods[i].draw(canvas);
    }
}

void Board::move(int from, int to, const bool log)
{
        // TODO 加一个操作是否合法的判断
        if (rods[from].top().val > rods[to].top().val)
        {
            std::cout << "Illegal move!" << std::endl;
            return;
        }

        rods[to].push(rods[from].top());
        rods[from].pop();
        ///手动执行 将这一对操作入栈 用于后面自动执行的时候还原
        if (log) history.push({from, to});
        //std::cout<<"Move disk from " << from << " to " << to << std::endl;
}

bool Board::win() const {
    if (rods[0].empty()&&rods[1].empty()&&rods[2].size()==num_disk )
    {
        return true; ///由于合法性在移动过程中就检验了 这里就不用了
    }
    else
    {
        return false;
    }
}

///在递归求解汉诺塔问题的过程中，将每一步的状态转换为字符串并存储到optimal_states中
void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution,
    std::unordered_set<std::string> &optimal_states,
    Board &board
) {
    if (n == 1) {
        solution.push({src, dest});
        board.move(src, dest, false); // 执行移动以更新状态
        optimal_states.insert(board.toString()); // 记录当前状态
        return;
    }
    solve(n - 1, src, dest, buf, solution, optimal_states, board);
    solution.push({src, dest});
    board.move(src, dest, false);
    optimal_states.insert(board.toString());
    solve(n - 1, buf, src, dest, solution, optimal_states, board);
}


//？由于没有传递参数 autoplay无法自身直接进行递归
//？借用上面写好的solve函数和辅助数据结构queue来实现
void Board::autoplay() {
    ///先一步一步复原
    ///注意复原时的顺序是反的
    while (!history.empty())
    {
        auto step = history.top();
        history.pop();
        std::cout<<"Auto moveing:"<<step.second<<"->"<<step.first<<std::endl;
        move(step.second, step.first, true);
        draw();
    }

    ///复原完自动
    Queue<std::pair<int, int>> solution;
    solve(num_disk, 0, 1, 2, solution, optimal_states, *this); // 调用 solve 函数

    while (!solution.empty()) {
        auto step = solution.front();
        solution.pop();
        move(step.first, step.second, true);
        draw(); // 打印当前状态
    }
}

void Board::hint() {
    // TODO 检查当前状态是否在递归路径上
    if (optimal_states.find(toString())!=optimal_states.end())
    {
        std::cout << "You are on the optimal path!" << std::endl;

    }
    else
    {
        std::cout << "Sorry. You are not on the optimal path." << std::endl;
    }
    draw();
    //？判断之后要再输出一次
}

std::string Board::toString() {
    std::string state;
    for (Rod &rod : rods) {
        state += rod.toString();
    }
    return state;
}