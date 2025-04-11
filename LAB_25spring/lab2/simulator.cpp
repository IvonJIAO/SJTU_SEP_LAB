#include "simulator.h"
#include <iostream>

Simulator::Simulator() : pc(0){
}

void Simulator::run(const int max_steps) {
    //* 0 step
    int step = 0;
    Status status = Status::AOK;

    //* >0 step
    for (step = 0; step < max_steps && status == Status::AOK; step++)
    {
        status = next_instruction();
    }
    report_stopped(step, status);
    //* 错误处理和最终状态输出都会存在
}

//* 获取高4位和低4位
static uint8_t get_hi4(const uint8_t pack) { return (pack >> 4) & 0xF; }
static uint8_t get_lo4(const uint8_t pack) { return pack & 0xF; }


//! run中的status从这里获取status


Status Simulator::next_instruction() {
    uint64_t next_pc = pc;

    //* get code and function (1 byte)
    //* 从内存中获取指令
    const std::optional<uint8_t> codefun = memory.get_byte(next_pc);
    if (!codefun) {
        report_bad_inst_addr();
        return Status::ADR;
    }

    //* 提取出icode和ifun
    const auto icode = static_cast<InstructionCode>(get_hi4(codefun.value()));
    uint8_t ifun = get_lo4(codefun.value());
    next_pc++;
    //? ++将使pc挪动一个字节
    //? ifun 只有部分指令需要 包括RRMOVQ，ALU，JMP

    uint8_t regA;
    uint8_t regB;
    // TODO: get registers if needed (1 byte)
    if (need_regids(icode))
    {
        //?这里使用了optional，找不到值会返回nullopt，类似false
        const std::optional<uint8_t> reg = memory.get_byte(next_pc);
        if (!reg) {
            report_bad_inst_addr();
            return Status::ADR;
        }
        regA = get_hi4(reg.value());
        regB = get_lo4(reg.value());
        next_pc++;

        //!修改了检查寄存器是否有效的逻辑 排除了一些普适情况 特殊情况在下面实现
        if (!error_invalid_reg(regA) && error_invalid_reg(regB)) {
            report_bad_reg(regB);
            return Status::INS;
        }
        if (error_invalid_reg(regA) && !error_invalid_reg(regB)) {
            report_bad_reg(regA);
            return Status::INS;
        }
        if (error_invalid_reg(regA) && error_invalid_reg(regB))
        {
        report_bad_reg(regA);
            return Status::INS;
        }
        if (regA == 0xF && regB == 0xF) {
            report_bad_reg(0xF);
            return Status::INS;
        }
        //!去掉了一个多余的赋值
    }

    // TODO: get immediate if needed (8 bytes)
    std::optional<uint64_t> imm;
    if (need_imm(icode)) {
        imm = memory.get_long(next_pc);
        // if (!imm) {
        //     report_bad_inst_addr();
        //     return Status::ADR;
        // }
        //!有的情况imm就是0？
        next_pc += 8;
    }

    // execute the instruction
    switch (icode) {
        ////---------------------------------------
        case InstructionCode::HALT: // 0:0
        {
            return Status::HLT;
        }

        ////-------------------------------------
        case InstructionCode::NOP: // 1:0
        {
            pc = next_pc;
            return Status::AOK;
        }

        ////-------------------------
        case InstructionCode::RRMOVQ: // 2:x regA:regB
            {
            if (error_invalid_reg(regA) || error_invalid_reg(regB)) {
                report_bad_reg(error_invalid_reg(regA) ? regA : regB);
                return Status::INS;
            }
            if (ConditionCodes().satisfy(static_cast<Condition>(ifun)))
            {
                registers[regB] = registers[regA];
                //! 这里一开始赋值反了！！
            }
            pc = next_pc;
            return Status::AOK;
        }

        ////-------------------------
        case InstructionCode::IRMOVQ: // 3:0 F:regB imm
            //* IRMOVQ 指令会将立即数（imm）加载到寄存器中
        // if (regB == 0xF) {
        //     report_bad_reg(0xF);
        //     return Status::INS;
        // }  已经被包含在最初的判断中了
            if (regA!= 0xF) {
                report_bad_reg(regA);
                return Status::INS;
            }
            if (!imm.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }
        {
            registers[regB] = imm.value();
            pc = next_pc;
            return Status::AOK;
        }

        ////--------------------------------
        case InstructionCode::RMMOVQ: // 4:0 regA:regB imm
        //* RMMOVQ 指令会将寄存器中的值存储
        // ?要求A,B,Memory都有效
        {
            if (error_invalid_reg(regA) || error_invalid_reg(regB)) {
                report_bad_reg(error_invalid_reg(regA) ? regA : regB);
                return Status::INS;
            }
            if (!imm.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            uint64_t address = registers[regB] + imm.value();
            if (!memory.set_long(address, registers[regA])) {
                report_bad_data_addr(address);
                return Status::ADR;
            }
            pc = next_pc;
            return Status::AOK;
        }

        ////-----------------------------------
        case InstructionCode::MRMOVQ: // 5:0 regA:regB imm
        {
            if (error_invalid_reg(regA) || error_invalid_reg(regB)) {
                report_bad_reg(error_invalid_reg(regA) ? regA : regB);
                return Status::INS;
            }
            if (!imm.has_value()) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            //* MRMOVQ 指令会将内存中指定地址的值加载到寄存器中
            uint64_t address = registers[regB] + imm.value();
            const std::optional<uint64_t> value = memory.get_long(address);
            if (!value) {
                report_bad_data_addr(address);
                return Status::ADR;
            }
            registers[regA] = value.value();
            pc = next_pc;
            return Status::AOK;
        }

        ////-------------------------------------------------
        case InstructionCode::ALU: // 6:x regA:regB
        {
            if (error_invalid_reg(regA) || error_invalid_reg(regB)) {
                report_bad_reg(error_invalid_reg(regA) ? regA : regB);
                return Status::INS;
            }
            uint64_t a = registers[regA];
            uint64_t b = registers[regB];
            uint64_t val = 0;
            switch (static_cast<AluOp>(ifun)) {
                case AluOp::ADD: val = a + b; break;
                case AluOp::SUB: val = b - a; break;
                case AluOp::AND: val = a & b; break;
                case AluOp::XOR: val = b ^ a; break;
                //! 这里的操作数顺序是 b op a!!
            }
            registers[regB] = val;
            cc = ConditionCodes::compute(static_cast<AluOp>(ifun), a, b, val);
            //std::cout<<"CC: "<<cc.name()<<std::endl;
            pc = next_pc;
            return Status::AOK;
        }

        ////-----------------------------------------------------
        case InstructionCode::JMP: // 7:x imm
        {
            if (!imm.has_value())
                {
                report_bad_inst_addr();
                return Status::ADR;
            }
            //在这一步帮我写一个输出CC值的语句
                //std::cout<<"currname"<<cc.name()<<std::endl;
            if (cc.satisfy(static_cast<Condition>(ifun))) //!这里使用cc 如果使用conditioncode() 会额外调用构造
            {
                pc = imm.value();
            }
            //*满足跳转 不满足跳过
            else pc = next_pc;
            return Status::AOK;
        }

        ////-----------------------------------------
        case InstructionCode::CALL: // 8:0 imm
            {
                if (!imm.has_value()) {
                    report_bad_inst_addr();
                    return Status::ADR;
                }
                // 将 next_pc 写入内存地址 %rsp - 8
                if (!memory.set_long(registers[RegId::RSP] - 8, next_pc)) {
                    report_bad_stack_addr(registers[RegId::RSP] - 8);
                    return Status::ADR;
                }
                // 更新 %rsp
                registers[RegId::RSP] -= 8;
                // 跳转到目标地址
                pc = imm.value();
                return Status::AOK;
            }

        ////-----------------------------------------
        case InstructionCode::RET: {
            // 9:0
            //* 从栈中弹出返回地址并跳转
            const std::optional<uint64_t> return_address = memory.get_long(registers[RegId::RSP]);
            if (!return_address) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            registers[RegId::RSP] += 8;  // 栈指针向上移动 8 字节
            pc = return_address.value();  // 设置返回地址为 PC
            return Status::AOK;
        }

        ////-----------------------------------------
        case InstructionCode::PUSHQ: // A:0 regA:F
        {
            if (error_invalid_reg(regA) || regB != 0xF) {
                report_bad_stack_addr(registers[regA]);
                return Status::INS;  // 如果 regB 不是 0xF 或 regA 无效，返回错误
            }

            // 获取 rA 寄存器的值
            uint64_t value_to_push = registers[regA];

            // // 检查寄存器是否为空
            // if (registers[regA] == 0) {
            //     report_bad_stack_addr(registers[regA] - 8);
            //     return Status::ADR;  // 如果寄存器为空，返回地址错误
            // }

            // !如果 regA 是 RSP，将旧的（没有减 8 的）数值压入栈中
            //! 但是内存的地址是更新的！！！
            if (regA == static_cast<uint8_t>(RegId::RSP)) {
                if (!memory.set_long(registers[RegId::RSP]-8, value_to_push)) {
                    report_bad_stack_addr(registers[RegId::RSP]-8);
                    return Status::ADR;
                }
                registers[RegId::RSP] -= 8;  // 更新 RSP
            } else {
                // 栈指针向下移动 8 字节

                if (!memory.set_long(registers[RegId::RSP]-8, value_to_push)) {
                    report_bad_stack_addr(registers[RegId::RSP]-8);
                    return Status::ADR;
                }
                registers[RegId::RSP] -= 8;
            }
            //! 将这里-8的顺序更改 同时报错的地址更改
            pc = next_pc;
            return Status::AOK;  // 指令执行成功
        }

        ////-----------------------------------------
        case InstructionCode::POPQ: // B:0 regA:F
        {
            if (error_invalid_reg(regA) || regB != 0xF) {
                report_bad_reg(registers[regA]);
                return Status::INS;  // 如果 regB 不是 0xF 或 regA 无效，返回错误
            }

            // 从栈中弹出数据
            std::optional<uint64_t> value_from_stack = memory.get_long(registers[RegId::RSP]);
            if (!value_from_stack) {
                report_bad_stack_addr(registers[RegId::RSP]);
                return Status::ADR;  // 如果内存读取失败，返回地址错误
            }

            // 将弹出的值保存到寄存器 rA
            registers[regA] = value_from_stack.value();

            // !如果 rA 是 RSP，将弹出的值保存到 RSP, 否则更新 RSP
            if (regA != static_cast<uint8_t>(RegId::RSP))
            {
                // 栈指针向上移动 8 字节
                registers[RegId::RSP] += 8;
            }
            pc = next_pc;
            return Status::AOK;  // 指令成功执行
        }


        ////-----------------------------------------
        default:
            report_bad_inst(codefun.value());
            return Status::INS;
    }
}

