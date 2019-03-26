#include "OperationFunctions.h"
#include <cmath>
#include <string>

bool isZero(double val)
{
    return std::abs(val) < eps ? true : false;
}

double calcAdd(double first, double second) {
    return first + second;
}

double calcSub(double first, double second) {
    return first - second;
}

double calcMul(double first, double second) {
    return first * second;
}

double calcDiv(double first, double second) {
    if (isZero(second)) {
        throw std::string("Error: division by zero");
    }
    return first / second;
}

double calcSin(double first) {
    double rad = first * Pi/180;
    return std::sin(rad);
}

double calcMinus(double first)
{
    return -first;
}

double calcCos(double first)
{
    double rad = first * Pi/180;
    return std::cos(rad);
}

double calcTan(double first)
{
    double rad = first * Pi/180;
    if (isZero(std::cos(rad))) {
        throw std::string("Error: tangent not defined");
    }
    return std::tan(rad);
}

double calcCtan(double first)
{
    double rad = first * Pi/180;
    double tangent = std::tan(rad);
    if (isZero(tangent)) {
        throw std::string("Error: cotangent not defined");
    }
    return 1 / tangent;
}

double calcPow(double first, double second)
{
    double res = std::pow(first, second);
    if (res == HUGE_VAL) {
        throw std::string("Error: exponentiation result too huge");
    }
    if (std::isnan(res)) {
        throw std::string("Error: uncorrect exponentiation range");
    }
    return res;
}
