#include "instructions.h"

#include <stdexcept>

#include <iostream>

bool need_regids(const InstructionCode icode) {
    switch (icode) {
        case InstructionCode::ALU:
        case InstructionCode::IRMOVQ:
        case InstructionCode::RRMOVQ:
        case InstructionCode::RMMOVQ:
        case InstructionCode::MRMOVQ:
        case InstructionCode::PUSHQ:
        case InstructionCode::POPQ:
            return true;
        case InstructionCode::RET:
        case InstructionCode::NOP:
        case InstructionCode::JMP:
        case InstructionCode::CALL:
        case InstructionCode::HALT:
            return false;

        default:
            throw std::logic_error("instructions.cpp: need_regids() not implemented");
    }
}

//!第二次测试后完善
bool need_imm(const InstructionCode icode) {
    switch (icode) {
        case InstructionCode::IRMOVQ:
        case InstructionCode::RMMOVQ:
        case InstructionCode::MRMOVQ:
        case InstructionCode::JMP:
        case InstructionCode::CALL:
            return true;
        case InstructionCode::ALU:
        case InstructionCode::RRMOVQ:
        case InstructionCode::NOP:
        case InstructionCode::HALT:
        case InstructionCode::RET:
        case InstructionCode::PUSHQ:
        case InstructionCode::POPQ:
            return false;

        default:
            throw std::logic_error("instructions.cpp: need_imm() not implemented");
    }
}


//!以下是对ALU操作一些函数的实现
ConditionCodes::ConditionCodes()
    : ConditionCodes(true, false, false) {
}


//* Constructor
//? 对三个bool值进行位运算，实现用一个int64_t来表示三个bool值
//? 压缩成一个uint8_t value
ConditionCodes::ConditionCodes(const bool zf, const bool sf, const bool of)
    : value((zf << 2) | (sf << 1) | (of << 0)) {
    //std::cout<<"constru:"<<name()<<std::endl;
}

static bool is_overflow(const AluOp op, const int64_t a, const int64_t b, const int64_t val) {
    switch (op) {
        case AluOp::ADD:
            return ((a < 0) == (b < 0)) && ((val < 0) != (a < 0));
        case AluOp::SUB:
            return ((a > 0) == (b < 0)) && ((val < 0) != (b < 0));
        default:
            return false;
    }
}

ConditionCodes ConditionCodes::compute(const AluOp op, const uint64_t a, const uint64_t b, const uint64_t val) {
    bool zero = (val == 0);
    bool sign = (static_cast<int64_t>(val) < 0);
    bool overflow = is_overflow(op, static_cast<int64_t>(a), static_cast<int64_t>(b), static_cast<int64_t>(val));
    //std::cout<<zero<<sign<<overflow<<std::endl;
    return {zero, sign, overflow};
    //?这一步执行了构造函数 ConditionCodes，并返回一个新的 ConditionCodes 对象
}

////这里操作计算不是很明白 问一下GPT
bool ConditionCodes::satisfy(const Condition cond) const {

    //std::cout<<"currval:"<<name()<<std::endl;
    const bool zf = (value >> 2) & 1;
    const bool sf = (value >> 1) & 1;
    const bool of = (value >> 0) & 1;

    //? sf:上一次的运算结果是否为负数
    //? of:上一次的运算结果是否溢出
    //? zf:上一次的运算结果是否为0
    //? 这里的运算符是位运算符，^表示异或，|表示或
    switch (cond) {
        case Condition::YES:
            //std::cout<<"YES"<<std::endl;
            return true;
        case Condition::LE:
            //std::cout<<"LE"<<std::endl;
            return (sf ^ of) | zf;
        case Condition::L:
            //std::cout<<"L"<<std::endl;
            return sf ^ of;
        case Condition::E:

            //std::cout<<"E "<<zf<<std::endl;
            return zf;
        case Condition::NE:
            //std::cout<<"NE"<<std::endl;
            return zf ^ 1;
        case Condition::GE:
            //std::cout<<"GE"<<std::endl;
            return sf ^ of ^ 1;
        case Condition::G:
            //std::cout<<"G"<<std::endl;
            return (sf ^ of ^ 1) & (zf ^ 1);
    }
    return false;
}

const char *ConditionCodes::name() const {
    switch (value) {
        case 0: return "Z=0 S=0 O=0";
        case 1: return "Z=0 S=0 O=1";
        case 2: return "Z=0 S=1 O=0";
        case 3: return "Z=0 S=1 O=1";
        case 4: return "Z=1 S=0 O=0";
        case 5: return "Z=1 S=0 O=1";
        case 6: return "Z=1 S=1 O=0";
        case 7: return "Z=1 S=1 O=1";
        default: return "???????????";
    }
}
