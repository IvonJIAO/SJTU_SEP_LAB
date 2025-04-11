#include <iostream>
#include <string>
#include <string_view>

#include "simulator.h"

static constexpr int MAX_STEP = 10000;

static void usage(const char *program)
{
    std::cout << "Usage: " << program << " file.bin [max_steps]\n";
}

int main(const int argc, char *argv[])


    //? 处理命令行参数
    //? argv[0]: 程序名
    //? argv[1]: 二进制文件名
    //? argv[2]: 最大步数（可选）
{
    //* 检查参数个数 如果参数个数小于2或者大于3（不合法），输出usage并返回0
    if (argc < 2 || argc > 3)
    {
        usage(argv[0]);
        return 0;
    }

    //* 检查参数个数 如果参数个数大于2，获取最大步数，否则设置为默认值MAX_STEP
    const int max_steps = argc > 2 ? std::stoi(argv[2]) : MAX_STEP;

    const char *filename = argv[1];
    //* 检查文件名是否以.bin结尾 如果不是，输出usage并返回0
    if (not std::string_view(filename).ends_with(".bin")) {
        usage(argv[0]);
        return 0;
    }

    Simulator sim;
    sim.load_binary(filename);

    //* 保留初始状态 最后在diff中调用
    const Registers save_registers = sim.get_registers();
    const Memory save_memory = sim.get_memory();

    sim.run(max_steps);
    //?集成到simulator类里的run函数
    //*这一步里要输出 停止步长 PC的停止地址 最后的状态 条件码的状态

    //? 输出改变的信息(其他的信息包含在了run函数里)
    std::cout << "Changes to registers:\n";
    Registers::diff(save_registers, sim.get_registers());

    std::cout << "\nChanges to memory:\n";
    Memory::diff(save_memory, sim.get_memory());

    return 0;
}

