#include <iostream>

#include "board.h"
#include "queue.h"

//！注意一下数组开的都是刚好大小的，编号均从0开始

Board::Board(const int num_disk)
    : num_disk(num_disk), disks(new Disk[num_disk]),
      rods{Rod(num_disk, 0), Rod(num_disk, 1), Rod(num_disk, 2)} {
    for (int i = num_disk-1; i >=0; i--)
    {
        disks[i] = Disk(i, 3 +2* i);
        rods[0].push(disks[i]);///所有盘子初始化到第0根杆上
    }
    //std::cout<<"Board constructor called."<<std::endl;
}

///析构应该只要把动态数组消了？
///delete[] 能够正确地释放数组的所有元素，并调用每个元素的析构函数（如果有
Board::~Board() {
    delete [] disks; //TODO 析构函数的实现
}

void Board::draw() {
    Canvas canvas {};
    //std::cout<<"Drawing board."<<std::endl;
    canvas.reset();
    for (int i = 0; i < ROD_SIZE; i++)
    {
        rods[i].draw(canvas); //！这里出现bug
        //std::cout<<"Drawing rod completed#"<<i<<std::endl;
    }
    canvas.draw();
}

void Board::move(int from, int to, const bool log)
{
       /* if ((rods[from].top().val&& rods[to].empty())||rods[from].top().val > rods[to].top().val)
        {
            std::cout << "Illegal move!" << std::endl;
            return;
        }
*/
    if (rods[from].empty() || (!rods[to].empty() && rods[from].top().val > rods[to].top().val)) {
        //std::cout << "Illegal move!" << std::endl;
        return;
    }
        rods[to].push(rods[from].top());
        rods[from].pop();
    //std::cout<<"Move disk from " << from << " to " << to << std::endl;
        ///手动执行 将这一对操作入栈 用于后面自动执行的时候还原
        if (log) history.push({from, to}); //！history没有push
        //std::cout<<"Move disk from " << from << " to " << to << std::endl;
}

bool Board::win() const {
    if (rods[0].empty()&&rods[2].empty()&&rods[1].size()==num_disk )
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
    //std::cout<<"Solving for n: " << n << ", src: " << src << ", buf: " << buf << ", dest: " << dest << std::endl;
    /*if (solution.empty())
        std::cout<<"Solution is empty!"<<std::endl;*/
    if (n == 1) {
        solution.push({src, dest});
        board.move(src, dest, false);
        //std::cout<<"Move disk from " <<std::endl;
        optimal_states.insert(board.toString()); // 记录当前状态
        return;
    }
    solve(n - 1, src, dest, buf, solution, optimal_states, board);
    solution.push({src, dest});
    /*if (solution.empty())
        std::cout<<"Solution is empty!after push"<<std::endl;*/
    board.move(src, dest, false);
    optimal_states.insert(board.toString());
    solve(n - 1, buf, src, dest, solution, optimal_states, board);
}


//？由于没有传递参数 autoplay无法自身直接进行递归
//？借用上面写好的solve函数和辅助数据结构queue来实现
void Board::autoplay() {
    //std::cout<<"Auto play started."<<std::endl;
    //draw();
    ///先一步一步复原
    ///注意复原时的顺序是反的
    while (!history.empty())
    {
        auto step = history.top();
        history.pop();//！这里没pop? no ！move 完又push进去了？
        //std::cout<<"History sz is:"<<history.size()<<std::endl;
        std::cout<<"Auto moving:"<<step.second+1<<"->"<<step.first+1<<std::endl;
        move(step.second, step.first, false);
        draw();
    }

    ///复原完自动
    Queue<std::pair<int, int>> solution;
    solve(num_disk, 0, 2, 1, solution, optimal_states, *this); // 调用 solve 函数

    /*if (solution.empty())
        std::cout<<"Solution is empty!"<<std::endl;
    else std::cout<<"Solution sz is:"<<solution.size()<<std::endl;
    */
//！调用完solve 应该重置各种rod的栈

    // 重置 rods 的状态
    for (int i = 0; i < ROD_SIZE; ++i) {
        while (!rods[i].empty()) {
            rods[i].pop();
        }
    }
    for (int i = num_disk - 1; i >= 0; --i) {
        rods[0].push(disks[i]);
    }



    while (!solution.empty()) {
        auto step = solution.front();
        solution.pop();
        //std::cout<<"before"<<std::endl;
        move(step.first, step.second, true);
        std::cout << "Auto moving:" << step.first+1 << "->" << step.second+1 << std::endl;
        draw();  //！move的是对的，但是draw的不对
    }
}

void Board::hint() {
    // 保存当前 rods 的状态
    Stack<Disk> temp_stacks[ROD_SIZE];
    for (int i = 0; i < ROD_SIZE; ++i) {
        while (!rods[i].empty()) {
            temp_stacks[i].push(rods[i].top());
            rods[i].pop();
        }
    }

    // 重置 rods 的状态
    for (int i = 0; i < ROD_SIZE; ++i) {
        while (!rods[i].empty()) {
            rods[i].pop();
        }
    }
    for (int i = num_disk - 1; i >= 0; --i) {
        rods[0].push(disks[i]);
    }

    optimal_states.insert(toString()); // 记录当前状态
    //draw();

    // 调用 solve 来获取 optimal_states
    Queue<std::pair<int, int>> solution;
    solve(num_disk, 0, 1, 2, solution, optimal_states, *this);

    // 恢复 rods 的状态
    for (int i = 0; i < ROD_SIZE; ++i) {
        while (!rods[i].empty()) {
            rods[i].pop();
        }
    }

    for (int i = 0; i < ROD_SIZE; ++i) {
        while (!temp_stacks[i].empty()) {
            rods[i].push(temp_stacks[i].top());
            temp_stacks[i].pop();
        }
    }

    // 检查 optimal_states 是否为空
    if (optimal_states.empty()) {
        std::cout << "Error: Optimal states have not been calculated." << std::endl;
        return;
    }

    // 检查当前状态是否在 optimal_states 中
    if (optimal_states.find(toString()) != optimal_states.end()) {
        std::cout << "You are on the optimal path!" << std::endl;
    } else {
        std::cout << "Sorry. You are not on the optimal path." << std::endl;
    }

    draw();

    // 输出当前状态的字符串表示
    //std::cout << "Current state: " << toString() << std::endl;
}

std::string Board::toString() {
    std::string state;
    for (Rod &rod : rods) {
        state += rod.toString();
    }
    return state;
}