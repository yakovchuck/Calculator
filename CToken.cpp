#include "CToken.h"
#include <sstream>

CToken::CToken(const std::string& lexStr, bool isNumber) :
    _isNumber(isNumber),
    _val(0),
    _lex(lexStr),
    _isOpenBlock(false),
    _isCloseBlock(false)
{
    if (_isNumber) {
        std::stringstream ss;
        ss << _lex;
        ss >> _val;
    } else {
        if (_lex == "(") {
            _isOpenBlock = true;
        } else if (_lex == ")") {
            _isCloseBlock = true;
        }
    }
}

CToken::CToken(double numberVal) :
    _isNumber(true),
    _val(numberVal),
    _lex(std::to_string(numberVal)),
    _isOpenBlock(false),
    _isCloseBlock(false)
{
    
}
