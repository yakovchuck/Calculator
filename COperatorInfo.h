#ifndef COperatorInfoH
#define COperatorInfoH

#include <string>
#include <functional>
#include <vector>

enum OperatorPriority {
    opLow = 1,
    opMiddle = 10,
    opBig = 50,
    opHuge = 100,
    opMax = 500
};

typedef std::function<double(double)> CalculationOperand1Func;
typedef std::function<double(double, double)> CalculationOperand2Func;

class CToken;
typedef std::shared_ptr<const CToken> CTokenConstPtr;
typedef std::vector<CTokenConstPtr> TTokens;

class COperatorInfo {
public:
    COperatorInfo(OperatorPriority priority, bool isRightAssociate, unsigned int operandsCount);
    OperatorPriority getPriority() const { return _priority; }
    bool isRightAssociate() const { return _isRightAssociate; }
    virtual void calc(TTokens& stackTokens) const = 0;
protected:
    unsigned int _operandsCount;
private:
    OperatorPriority _priority;
    bool _isRightAssociate;
};

typedef std::shared_ptr<COperatorInfo> COperatorInfoPtr;
typedef std::shared_ptr<const COperatorInfo> COperatorInfoConstPtr;

class COperatorOperand1Info : public COperatorInfo
{
public:
    COperatorOperand1Info(OperatorPriority priority, bool isRightAssociate, CalculationOperand1Func calculationOperand1Func);
    virtual void calc(TTokens& stackTokens) const;
private:
    CalculationOperand1Func _calculationOperand1Func;
};

class COperatorOperand2Info : public COperatorInfo
{
public:
    COperatorOperand2Info(OperatorPriority priority, bool isRightAssociate, CalculationOperand2Func calculationOperand2Func);
    virtual void calc(TTokens& stackTokens) const;
private:
    CalculationOperand2Func _calculationOperand2Func;
};

#endif
