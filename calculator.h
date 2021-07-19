#pragma once

#include <stack>
#include <string>
#include <string>

class Calculator
{
public:
    double Evaluate(std::string expression);

private:
    void DoOperation(char c);
    void HandleClosingBracket(bool& canBeUnary);

private:
    std::stack<double> numbers;
    std::stack<char> operations;

};