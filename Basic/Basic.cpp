/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
            break;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string tmp = scanner.nextToken();
    if (scanner.getTokenType(tmp) == NUMBER) {
        int linenumber = std::stoi(tmp); ///Have some risk!!
        std::string line;
        while (scanner.hasMoreTokens()) {
            line += scanner.nextToken() + ' ';
        }
        program.addSourceLine(linenumber, line);
        return;
    } else {
        if (tmp == "LET") {
            if (!scanner.hasMoreTokens()) {
                std::cout << "SYNTAX ERROR" << std::endl;
                return;
            } else {
                std::string variable = scanner.nextToken();
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
                        sta->execute(state, program);
                        delete sta;
                        return;
                    } else {
                        std::cout << "SYNTAX ERROR" << std::endl;
                        return;
                    }
                } else {
                    std::cout << "SYNTAX ERROR" << std::endl;
                    return;
                }
            }
            return;
        }
        if (tmp == "PRINT")
        {
            if (!scanner.hasMoreTokens()) {
                std::cout << "SYNTAX ERROR" << std::endl;
                return;
            } else {
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
                sta->execute(state, program);
                delete sta;
                return;
            }
            return;
        }
        if (tmp == "INPUT")
        {
            if (!scanner.hasMoreTokens()) {
                std::cout << "SYNTAX ERROR" << std::endl;
                return;
            } else {
                std::string variable = scanner.nextToken();
                if (scanner.hasMoreTokens()) {
                    std::cout << "SYNTAX ERROR" << std::endl;
                    return;
                }
                Statement *sta = new InputStatement(variable);
                sta->execute(state, program);
                delete sta;
                return;
            }
            return;
        }
        if (tmp == "END") {
            Statement *sta = new EndStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        if (tmp == "REM") {
            Statement *sta = new REMStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        if (tmp == "RUN") {
            Statement *sta = new RunStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        if (tmp == "LIST") {
            Statement *sta = new ListStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        if (tmp == "CLEAR") {
            Statement *sta = new ClearStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        if (tmp == "QUIT") {
            Statement *sta = new QuitStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        if (tmp == "HELP") {
            Statement *sta = new HelpStatement();
            sta->execute(state, program);
            delete sta;
            return;
        }
        std::cout << "SYNTAX ERROR" << std::endl;
        return;
    }
}

