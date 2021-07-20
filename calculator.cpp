#include "calculator.h"
#include <iostream>
#include <regex>

bool IsUnary(char c) //unary minus
{
    return c == '#';
}

bool IsDigit(char c)
{
    return '0' <= c && c <= '9';
}

bool IsNumberSymbol(char c)
{
    return c == '.' || c == ',' || IsDigit(c);
}

bool IsOperationSymbol(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

int GetOperationPriority(char c) //higher return value = higher priority
{
    switch (c)
    {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '#':
            return 3;
        default:
            return 0;
    }
}

std::string delSpaceAndCommas(std::string expression_)
{
    std::string expression;
    expression.reserve(expression.size() + 2);
    expression.append("(");

    for (const char c : expression_)
    {
        if (c == ',')
        {
            expression.push_back('.');
        }
        else if (c != ' ')
        {
            expression.push_back(c);
        }
    }

    expression.append(")");
    return expression;
}

bool hasWhitespacesBetweenNumbers(std::string expression) //white spaces between numbers is an error (eg. "1 2 + 3" - invalid)
{
    char lastMeaningSymbol = '%';

    for (int i = 0; i < expression.size(); ++i) 
    {
        if (IsNumberSymbol(lastMeaningSymbol) && IsNumberSymbol(expression[i]) && (expression[i - 1] == ' ')) 
            return true;

        if (expression[i] != ' ') 
        {
            lastMeaningSymbol = expression[i];
        }
    }

    return false;
}

double Calculator::Evaluate(std::string expression_)
{
    if (expression_.empty())
        throw std::runtime_error("Empty expression");

    if (hasWhitespacesBetweenNumbers(expression_))
        throw std::runtime_error("Expression has whitespaces between numbers");
    
    std::string expression = delSpaceAndCommas(expression_);

    numbers = std::stack<double>(); //clear stacks
    operations = std::stack<char>();

    bool canBeUnary = true; //if it can be unary operation now

    int i = 0;
    while (i < expression.size())
    {
        char c = expression[i];

        if (IsNumberSymbol(c))
        {
            canBeUnary = false;
            int position = expression.find_first_not_of("0123456789.", i);
            if (position == std::string::npos)
            {
                position = expression.size();
            }
            std::string possibleNumber = expression.substr(i, position - i);
            std::regex regex("^[0-9]+(.[0-9]+)?$"); //regex for correct int or float number

            if (!std::regex_match(possibleNumber, regex))
            {
                throw std::runtime_error(possibleNumber + "is not a number");
            }
                        
            numbers.push(std::atof(possibleNumber.data())); //number to float using atof

            i = position;
            continue;
        }

        if (!IsOperationSymbol(c))
        {
            throw std::runtime_error(std::string("Unsupported expression symbol ") + c);
            
        }
        
        if (c == '(') 
        {
            operations.push(c);
            canBeUnary = true;
            ++i;
            continue;
        }

        if (c == ')') 
        {
            HandleClosingBracket(canBeUnary);
            ++i;
            continue;
        }

        if (canBeUnary && c == '-') 
        {
            c = '#';
        }

        while (!operations.empty()) //do all operations having higher priority than current
        {
            char lastOperation = operations.top();
            operations.pop();

            if (GetOperationPriority(lastOperation) >= GetOperationPriority(c)) 
            {
                DoOperation(lastOperation);
            }
            else 
            {
                operations.push(lastOperation);
                break;
            }
        }

        operations.push(c);
        canBeUnary = true;
        ++i;
    }
    
    if (numbers.size() != 1 || !operations.empty()) 
    {
        throw std::runtime_error("Invalid expression");
    }

    return numbers.top();
}

void Calculator::DoOperation(char c)
{
    if (IsUnary(c)) 
    {
        if (numbers.empty()) 
        {
            throw std::runtime_error("Not enough operands for unary operation");
        }

        const double number = numbers.top();

        if (c == '#') 
        {
            numbers.pop();
            numbers.push(-number);
        }

        return;
    }

    if (numbers.size() < 2) 
    {
        throw std::runtime_error(std::string("Not enough operands for operation ") + c);
    }

    const double second = numbers.top();
    numbers.pop();
    const double first = numbers.top();
    numbers.pop();

    switch (c)
    {
        case '+': numbers.push(first + second); break;
        case '-': numbers.push(first - second); break;
        case '*': numbers.push(first * second); break;
        case '/': numbers.push(first / second); break;
        default: throw std::runtime_error("Unsupported operation");
    }
}

void Calculator::HandleClosingBracket(bool& canBeUnary)
{
    char lastOperation = '%';
    while (!operations.empty()) 
    {
        lastOperation = operations.top();
        operations.pop();

        if (lastOperation == '(') 
        {
            break;
        }

        DoOperation(lastOperation);
    }

    canBeUnary = false;

    if (lastOperation != '(') 
    {
        throw std::runtime_error("Invalid brackets balance");
    }
}
