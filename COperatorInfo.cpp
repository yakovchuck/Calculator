#include "COperatorInfo.h"
#include "CToken.h"

COperatorInfo::COperatorInfo(OperatorPriority priority, bool isRightAssociate, unsigned int operandsCount) :
    _operandsCount(operandsCount),
    _priority(priority),
    _isRightAssociate(isRightAssociate)
{
    
}

COperatorOperand1Info::COperatorOperand1Info(OperatorPriority priority, bool isRightAssociate, CalculationOperand1Func calculationOperand1Func) :
    COperatorInfo(priority, isRightAssociate, 1),
    _calculationOperand1Func(calculationOperand1Func)
{
    
}

void COperatorOperand1Info::calc(TTokens& stackTokens) const
{
    if (stackTokens.empty()) {
        throw std::string("Error: calculation error");
    }
    
    double first = stackTokens.back()->getVal();
    stackTokens.pop_back();
    
    double res = _calculationOperand1Func(first);
    stackTokens.push_back(std::make_shared<CToken>(res));
}

COperatorOperand2Info::COperatorOperand2Info(OperatorPriority priority, bool isRightAssociate, CalculationOperand2Func calculationOperand2Func) :
    COperatorInfo(priority, isRightAssociate, 2),
    _calculationOperand2Func(calculationOperand2Func)
{
    
}

void COperatorOperand2Info::calc(TTokens& stackTokens) const
{
    if (stackTokens.empty() || stackTokens.size() == 1) {
        throw std::string("Error: calculation error");
    }
    
    double second = stackTokens.back()->getVal();
    stackTokens.pop_back();
    
    double first = stackTokens.back()->getVal();
    stackTokens.pop_back();
    
    double res = _calculationOperand2Func(first, second);
    stackTokens.push_back(std::make_shared<CToken>(res));
}
