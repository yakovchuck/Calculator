#ifndef CTokenH
#define CTokenH

#include <string>

class CToken {
public:
    CToken(const std::string& lexStr, bool isNumber);
    CToken(double numberVal);
    bool isNumber() const { return _isNumber; }
    double getVal() const { return _val; }
    const std::string& getLex() const { return _lex; }
    bool isOpenBlock() const { return _isOpenBlock; }
    bool isCloseBlock() const { return _isCloseBlock; }
private:
    bool _isNumber;
    double _val;
    std::string _lex;
    bool _isOpenBlock;
    bool _isCloseBlock;
};

typedef std::shared_ptr<CToken> CTokenPtr;
typedef std::shared_ptr<const CToken> CTokenConstPtr;

#endif
