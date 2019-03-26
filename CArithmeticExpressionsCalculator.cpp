#include "CArithmeticExpressionsCalculator.h"
#include "COperatorInfo.h"
#include "CToken.h"
#include "OperationFunctions.h"
#include <sstream>
#include <vector>

namespace {
    const char* minusLex = "minus";
}

CArithmeticExpressionsCalculator::CArithmeticExpressionsCalculator()
{
    addAviableOperatorInfo("+", std::make_shared<COperatorOperand2Info>(opLow, false, std::bind(&calcAdd, std::placeholders::_1, std::placeholders::_2)));
	addAviableOperatorInfo("-", std::make_shared<COperatorOperand2Info>(opLow, false, std::bind(&calcSub, std::placeholders::_1, std::placeholders::_2)));
    addAviableOperatorInfo(minusLex, std::make_shared<COperatorOperand1Info>(opMax, true, std::bind(&calcMinus, std::placeholders::_1)));
	addAviableOperatorInfo("*", std::make_shared<COperatorOperand2Info>(opMiddle, false, std::bind(&calcMul, std::placeholders::_1, std::placeholders::_2)));
	addAviableOperatorInfo("/", std::make_shared<COperatorOperand2Info>(opMiddle, false, std::bind(&calcDiv, std::placeholders::_1, std::placeholders::_2)));
    addAviableOperatorInfo("sin", std::make_shared<COperatorOperand1Info>(opBig, true, std::bind(&calcSin, std::placeholders::_1)));
    addAviableOperatorInfo("cos", std::make_shared<COperatorOperand1Info>(opBig, true, std::bind(&calcCos, std::placeholders::_1)));
    addAviableOperatorInfo("tan", std::make_shared<COperatorOperand1Info>(opBig, true, std::bind(&calcTan, std::placeholders::_1)));
    addAviableOperatorInfo("ctan", std::make_shared<COperatorOperand1Info>(opBig, true, std::bind(&calcCtan, std::placeholders::_1)));
        addAviableOperatorInfo("^", std::make_shared<COperatorOperand2Info>(opHuge, false, std::bind(&calcPow, std::placeholders::_1, std::placeholders::_2)));
}

double CArithmeticExpressionsCalculator::calculate(const std::string& arithExprStr)
{
    // Делим строку на отдельные лексемы
    std::vector<CTokenConstPtr> tokens;
    
    ConstStringIterator itExpr = arithExprStr.begin();
    while (itExpr != arithExprStr.end()) {
        if (*itExpr == ' ') {
            ++itExpr;
            continue;
        } else if (*itExpr == '(') {
            ++itExpr;
            tokens.push_back(std::make_shared<CToken>("(", false));
        } else if (*itExpr == ')') {
            ++itExpr;
            tokens.push_back(std::make_shared<CToken>(")", false));
        } else {
            CTokenConstPtr token = createToken(itExpr, arithExprStr);
            tokens.push_back(token);
        }
    }
    
    bool numberTokenExist = false;
    for (std::vector<CTokenConstPtr>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if ((*it)->isNumber()) {
            numberTokenExist = true;
            break;
        }
    }
    
    //Строим обратную польскую последовательность
    std::vector<CTokenConstPtr> rpnTokens;
    std::vector<CTokenConstPtr> stackTokens;
    
    for (std::vector<CTokenConstPtr>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        if ((*it)->isNumber()) {
            rpnTokens.push_back(*it);
        } else if ((*it)->isOpenBlock()) {
            stackTokens.push_back(*it);
        } else if ((*it)->isCloseBlock()) {
            while (!stackTokens.empty() && !stackTokens.back()->isOpenBlock()) {
                rpnTokens.push_back(stackTokens.back());
                stackTokens.pop_back();
            }
            if (!stackTokens.empty() && stackTokens.back()->isOpenBlock()) {
                stackTokens.pop_back();
            } else {
                throw std::string("Error: uncorrect block");
            }
        } else {
            while (!stackTokens.empty() && isNeedPopFromStack(stackTokens.back(), *it)) {
                rpnTokens.push_back(stackTokens.back());
                stackTokens.pop_back();
            }
            stackTokens.push_back(*it);
        }
    }
    
    while (!stackTokens.empty()) {
        if (stackTokens.back()->isOpenBlock()) {
            throw std::string("Error: uncorrect block");
        }
        rpnTokens.push_back(stackTokens.back());
        stackTokens.pop_back();
    }
    
    //Считаем выражение
    for (std::vector<CTokenConstPtr>::iterator it = rpnTokens.begin(); it != rpnTokens.end(); ++it) {
        if ((*it)->isNumber()) {
            stackTokens.push_back(*it);
        } else {
            TOperatorInfoMap::iterator infoIt = _operatorsInfo.find((*it)->getLex());
            if (infoIt != _operatorsInfo.end()) {
                infoIt->second->calc(stackTokens);
            } else {
                throw std::string("Error: undefined operator");
            }
        }
    }
    
    if (stackTokens.empty() || stackTokens.size() > 1) {
        throw std::string("Error: uncorrect result");
    }
    
    return stackTokens.back()->getVal();
}

bool CArithmeticExpressionsCalculator::isNeedPopFromStack(const CTokenConstPtr& stackToken, const CTokenConstPtr& curToken) const
{
    if (stackToken->isOpenBlock()) {
        return false;
    }
    
    TOperatorInfoMap::const_iterator stackOperatorInfoIt = _operatorsInfo.find(stackToken->getLex());
    if (stackOperatorInfoIt == _operatorsInfo.end()) {
        throw std::string("Error: undefined operator");
    }
    
    TOperatorInfoMap::const_iterator curOperatorInfoIt = _operatorsInfo.find(curToken->getLex());
    if (curOperatorInfoIt == _operatorsInfo.end()) {
        throw std::string("Error: undefined operator");
    }
    
    if ((curOperatorInfoIt->second->isRightAssociate() && curOperatorInfoIt->second->getPriority() < stackOperatorInfoIt->second->getPriority()) || (!curOperatorInfoIt->second->isRightAssociate() && curOperatorInfoIt->second->getPriority() <= stackOperatorInfoIt->second->getPriority())) {
        return true;
    } else {
        return false;
    }
}

CTokenConstPtr CArithmeticExpressionsCalculator::createToken(ConstStringIterator& curIt, const std::string& arithExprStr)
{
    std::stringstream lex;
    
    bool numberLex = false;
    if (isNumber(*curIt)) {
        numberLex = true;
    }
    
    bool wasDec = false;
    
    //Отдельно проверим унарный минус (т.к. имеется одинаковая лексема для унарной и бинарной операции)
    if (!arithExprStr.empty() && !numberLex && *curIt == '-') {
        ConstStringIterator checkIt = curIt;
        do {
            --checkIt;
            if (*checkIt == ' ') {
                continue;
            } else {
                if (!isNumber(*checkIt) && *checkIt != ')') {
                    if (_operatorsInfo.find(minusLex) == _operatorsInfo.end()) {
                        throw std::string("Error: unary minus not defined");
                    }
                    ++curIt;
                    return std::make_shared<CToken>(minusLex, false);
                } else {
                    break;
                }
            }
        } while (checkIt != arithExprStr.begin());
    }
    
    //Строим токен
    for (; curIt != arithExprStr.end(); ++curIt) {
        if (*curIt == ' ') {
            continue;
        }
        
        if (*curIt == '(' || *curIt == ')') {
            break;
        }
        
        bool isCurCharForNumber = isNumber(*curIt);
        std::string str = lex.str();
        std::string lowerStr;
        std::transform(str.begin(), str.end(), std::back_inserter(lowerStr), tolower);
        if ((numberLex && !isCurCharForNumber) || (!numberLex && isCurCharForNumber) || (!numberLex && _operatorsInfo.find(lowerStr) != _operatorsInfo.end())) {
            break;
        }
        
        if (numberLex && (*curIt == '.' || *curIt == ',')) {
            if (wasDec) {
                throw std::string("Error: uncorrect number");
            } else {
                lex << ".";
                wasDec = true;
                continue;
            }
        }
        
        lex << *curIt;
    }
    
    std::string lexStr = lex.str();
    std::string lowerLexStr;
    std::transform(lexStr.begin(), lexStr.end(), std::back_inserter(lowerLexStr), tolower);
    
    if (!numberLex && _operatorsInfo.find(lowerLexStr) == _operatorsInfo.end()) {
        throw std::string("Error: undefined operator");
    }
    
    return std::make_shared<CToken>(lowerLexStr, numberLex);
}

void CArithmeticExpressionsCalculator::addAviableOperatorInfo(const std::string& operatorLex, COperatorInfoConstPtr operatorInfo)
{
	_operatorsInfo[operatorLex] = operatorInfo;
}

bool CArithmeticExpressionsCalculator::isNumber(char c) const
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '.' || c == ',') {
        return true;
    } else {
        return false;
    }
}
