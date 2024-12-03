/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    program.clear();
    return;
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    TokenScanner scanner;
    scanner.ignoreComments();
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    if (scanner.hasMoreTokens()) {
        std::string token = scanner.nextToken();
        if (token == "REM") {
            Statement *sta = new REMStatement();
            program[lineNumber] = {sta, line};
            return;
        }
        if (token == "LET") {
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            } else {
                std::string variable = scanner.nextToken();
                if (variable == "LET")
                {
                    error("SYNTAX ERROR");
                }
                if (scanner.nextToken()[0] == '=') {
                    if (scanner.hasMoreTokens()) {
                        std::string ex;
                        while (scanner.hasMoreTokens()) {
                            ex += scanner.nextToken() + ' ';
                        }
                        TokenScanner expp;
                        expp.ignoreWhitespace();
                        expp.scanNumbers();
                        expp.setInput(ex);
                        Expression *exp = parseExp(expp);
                        Statement *sta = new LetStatement(variable, exp);
                        program[lineNumber] = {sta, line};
                    } else {
                        error("SYNTAX ERROR");
                    }
                }
            }
            return;
        }
        if (token == "GOTO") {
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            }
            int value;
            std::string input = scanner.nextToken();
            if (scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            }
            try {
                value = std::stoi(input);
                Statement *sta = new GotoStatement(value);
                program[lineNumber] = {sta, line};
            } 
            catch(std::exception& e) {
                error("SYNTAX ERROR");
            }
            return;
        }
        if (token == "IF") {
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            } else {
                std::string left;
                std::string right;
                char op;
                int target;
                while (true) {
                    std::string tmp = scanner.nextToken();
                    if (tmp[0] == '=' || tmp[0] == '<' || tmp[0] == '>') {
                        op = tmp[0];
                        break;
                    }
                    left += tmp + ' ';
                }
                TokenScanner expp;
                expp.ignoreWhitespace();
                expp.scanNumbers();
                expp.setInput(left);
                Expression *lhs = parseExp(expp);
                op = scanner.nextToken()[0];
                while (true) {
                    std::string tmp = scanner.nextToken();
                    if (tmp == "THEN") {
                        break;
                    }
                    right += tmp + ' ';
                }
                TokenScanner exppp;
                exppp.ignoreWhitespace();
                exppp.scanNumbers();
                exppp.setInput(right);
                Expression *rhs = parseExp(exppp);
                std::string ttmp = scanner.nextToken();
                try {
                    target = std::stoi(ttmp);
                } 
                catch(std::exception& e) {
                    error("INVALID NUMBER");
                }
                if (scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR");
                }
                Statement *sta = new IfStatement(lhs, rhs, op, target);
                program[lineNumber] = {sta, line};
            }
            return;
        }
        if (token == "PRINT") {
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            }
            else {
                std::string ex;
                while (scanner.hasMoreTokens()) {
                    ex += scanner.nextToken() + ' ';
                }
                TokenScanner expp;
                expp.ignoreWhitespace();
                expp.scanNumbers();
                expp.setInput(ex);
                Expression *exp = parseExp(expp);
                Statement *sta = new PrintStatement(exp);
                program[lineNumber] = {sta, line};
            }
            return;
        }
        if (token == "INPUT") {
            if (!scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            }
            else {
                std::string variable = scanner.nextToken();
                if (scanner.hasMoreTokens()) {
                    error("SYNTAX ERROR");
                }
                Statement *sta = new InputStatement(variable);
                program[lineNumber] = {sta, line};
            }
            return;
        }
        if (token == "END") {
            if (scanner.hasMoreTokens()) {
                error("SYNTAX ERROR");
            }
            Statement *sta = new EndStatement();
            program[lineNumber] = {sta, line};
            return;
        }
        error("SYNTAX ERROR");
    }
    else {
        if (program.find(lineNumber) == program.end()) {
            return;
        } else {
            program.erase(lineNumber);
            return;
        }
    }
    
}

void Program::removeSourceLine(int lineNumber) {
    auto ff = program.find(lineNumber);
    if (ff == program.end()) {
        return;
    } else {
        program.erase(ff);
        return;
    }
}

std::string Program::getSourceLine(int lineNumber) {
    auto ff = program.find(lineNumber);
    if (ff == program.end()) {
        return "";
    }
    return ff->second.second;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    auto ff = program.find(lineNumber);
    if (ff == program.end()) {
        error("LINE NUMBER ERROR");
        return;
    }
    ff->second.first = stmt;
    return;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    auto ff = program.find(lineNumber);
    if (ff == program.end()) {
        return nullptr;
    }
    Statement *sta = ff->second.first;
    return sta;
}

int Program::getFirstLineNumber() {
    auto minimal = this->program.begin();
    if (minimal == program.end()) {
        return -1;
    }
    int value = minimal->first;
    return value;
}

int Program::getNextLineNumber(int lineNumber) {
    auto ff = program.find(lineNumber);
    if (ff == program.end()) {
        return -1;
    }
    auto nextff = std::next(ff);
    if (nextff == program.end()) {
        return -1;
    }
    int value = nextff->first;
    return value;
}

//more func to add
//todo


