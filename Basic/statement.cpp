/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

Statement::Statement() = default;

Statement::~Statement() = default;

//todo
REMStatement::REMStatement() {}
void REMStatement::execute(EvalState &state, Program &pro) {
    return;
}

LetStatement::LetStatement(std::string variable, Expression *exp) : var(variable), exp(exp) {}
LetStatement::~LetStatement() {
    delete exp;
}
void LetStatement::execute(EvalState &state, Program &pro) {
    if (var == "LET")
    {
        std::cout << "SYNTAX ERROR" << std::endl;
        return;
    }
    int value = exp->eval(state);
    state.setValue(var, value);
    return;
}

PrintStatement::PrintStatement(Expression *exp) : exp(exp) {}
PrintStatement::~PrintStatement() {
    delete exp;
}
void PrintStatement::execute(EvalState &state, Program &pro) {
    try
    {
        int value = exp->eval(state);
        std::cout << value << std::endl;
    }
    catch(std::exception& e)
    {
        delete exp;
        throw;
    }
    return;
}

InputStatement::InputStatement(std::string str) : variable(str) {}
void InputStatement::execute(EvalState &state, Program &pro) {
    int value;
    while (true)
    {
        std::cout << " ? ";
        std::string input;
        getline(std::cin, input);
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(input);
        std::string token = scanner.nextToken();
        bool negative = false;
        if (token[0] == '-')
        {
            token = scanner.nextToken();
            negative = true;
        }
        if (scanner.hasMoreTokens() || scanner.getTokenType(token) != NUMBER) {
            std::cout << "INVALID NUMBER" << std::endl;
            continue;
        } else {
            bool flag = true;
            for (int i = 0; i < token.length(); i++)
            {
                if (token[i] == '.')
                {
                    flag = false;
                }
            }
            if (flag)
            {
                value = std::stoi(token);
                if (negative)
                {
                    value = -value;
                }
                break;
            }
            else {
                std::cout << "INVALID NUMBER" << std::endl;
                continue;
            }
        }
    }
    state.setValue(variable, value);
}

EndStatement::EndStatement() {}
void EndStatement::execute(EvalState &state, Program &pro) {}

GotoStatement::GotoStatement(int line) : linetarget(line) {}
void GotoStatement::execute(EvalState &state, Program &pro) {
    if (pro.getSourceLine(linetarget).empty())
    {
        error("LINE NUMBER ERROR");
    }
}
int GotoStatement::getlinetarget() {
    return linetarget;
}

IfStatement::IfStatement(Expression *left, Expression *right, char s, int targetline) : lhs(left), rhs(right), op(s), target(targetline) {}
IfStatement::~IfStatement() {
    delete lhs;
    delete rhs;
}
void IfStatement::execute(EvalState &state, Program &pro) {
    if (pro.getSourceLine(target).empty())
    {
        error("LINE NUMBER ERROR");
    }
    return;
}
bool IfStatement::condition(EvalState &state, Program &pro) {
    bool flag = false;
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    if (op == '=')
    {
        flag = (left == right);
    }
    if (op == '<')
    {
        flag = (left < right);
    }
    if (op == '>')
    {
        flag = (left > right);
    }
    return flag;
}
int IfStatement::get_target_line() {
    return target;
}

RunStatement::RunStatement() {}
void RunStatement::execute(EvalState &state, Program &pro) {
    int cur_line_number = pro.getFirstLineNumber();
    if (cur_line_number == -1)
    {
        return;
    }
    while (cur_line_number != -1)
    {
        Statement *sta = pro.getParsedStatement(cur_line_number);
        if (sta == nullptr)
        {
            cur_line_number = pro.getNextLineNumber(cur_line_number);
            continue;
        }
        if (pro.getSourceLine(cur_line_number)[0] == 'G')
        {
            sta->execute(state, pro);
            GotoStatement *newsta = dynamic_cast<GotoStatement *>(sta);
            cur_line_number = newsta->getlinetarget();
            continue;
        }
        if (pro.getSourceLine(cur_line_number)[0] == 'I' && pro.getSourceLine(cur_line_number)[1] == 'F')
        {
            sta->execute(state, pro);
            IfStatement *newsta = dynamic_cast<IfStatement *>(sta);
            if (newsta->condition(state, pro)) {
                cur_line_number = newsta->get_target_line();
            }
            else {
                cur_line_number = pro.getNextLineNumber(cur_line_number);
            }
            continue;
        }
        if (pro.getSourceLine(cur_line_number)[0] == 'E') {
            break;
        }
        sta->execute(state, pro);
        cur_line_number = pro.getNextLineNumber(cur_line_number);
    }
}

ListStatement::ListStatement() {}
void ListStatement::execute(EvalState &state, Program &pro) {
    int cur_line_number = pro.getFirstLineNumber();
    if (cur_line_number == -1)
    {
        return;
    }
    while (cur_line_number != -1)
    {
        std::cout << cur_line_number << ' ';
        std::cout << pro.getSourceLine(cur_line_number) << std::endl;
        cur_line_number = pro.getNextLineNumber(cur_line_number);
    }
}

QuitStatement::QuitStatement() {}
void QuitStatement::execute(EvalState &state, Program &pro) {
    pro.~Program();
    state.~EvalState();
    exit(0);
}

ClearStatement::ClearStatement() {}
void ClearStatement::execute(EvalState &state, Program &pro) {
    pro.~Program();
    state.~EvalState();
    pro.clear();
    state.Clear();
}

HelpStatement::HelpStatement() {}
void HelpStatement::execute(EvalState &state, Program &pro) {
    std::cout << "THANK GOD" << std::endl;
}

