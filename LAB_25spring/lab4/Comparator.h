#pragma once
#include <cmath>

//? 设置用于浮点数比较的全局精度
constexpr double EPSILON = 1e-6;

//? inline function : insert the code block into the program
//* to compare the floating point numbers, we cannot simply use the "==" operator
inline bool isEqual(double a, double b) { return std::fabs(a - b) < EPSILON; }

inline bool isLessThan(double a, double b) { return (b - a) > EPSILON; }

inline bool isGreaterThan(double a, double b) { return (a - b) > EPSILON; }

inline bool isLessThanOrEqual(double a, double b) {
  return !isGreaterThan(a, b);
}

inline bool isGreaterThanOrEqual(double a, double b) {
  return !isLessThan(a, b);
}
