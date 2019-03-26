#ifndef CArithmeticExpressionsCalculatorH
#define CArithmeticExpressionsCalculatorH

#include <string>
#include <map>

typedef std::string::const_iterator ConstStringIterator;

class COperatorInfo;
typedef std::shared_ptr<const COperatorInfo> COperatorInfoConstPtr;

typedef std::map<std::string, COperatorInfoConstPtr> TOperatorInfoMap;

class CToken;
typedef std::shared_ptr<const CToken> CTokenConstPtr;

class CArithmeticExpressionsCalculator {
public:
	CArithmeticExpressionsCalculator();
	double calculate(const std::string& arithExprStr);
private:
	void addAviableOperatorInfo(const std::string& operatorLex, COperatorInfoConstPtr operatorInfo);
    bool isNumber(char c) const;
    CTokenConstPtr createToken(ConstStringIterator& curIt, const std::string& arithExprStr);
    bool isNeedPopFromStack(const CTokenConstPtr& stackToken, const CTokenConstPtr& curToken) const;
private:
	TOperatorInfoMap _operatorsInfo;
};

#endif
