/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "program.hpp"
#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &pro) = 0;

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */
class REMStatement : public Statement {
    public:
    REMStatement();
    void execute(EvalState &state, Program &pro);
};

class LetStatement : public Statement {
    public:
    LetStatement(std::string variable, Expression *exp);
    ~LetStatement();
    void execute(EvalState &state, Program &pro);
    private:
    std::string var;
    Expression *exp;
};

class PrintStatement : public Statement {
    public:
    PrintStatement(Expression *exp);
    ~PrintStatement();
    void execute(EvalState &state, Program &pro);
    private:
    Expression *exp;
};

class InputStatement : public Statement {
    private:
    std::string variable;
    public:
    InputStatement(std::string str);
    void execute(EvalState &state, Program &pro);
};

class EndStatement : public Statement {
    public:
    EndStatement();
    void execute(EvalState &state, Program &pro);
};

class GotoStatement : public Statement {
    private:
    int linetarget;
    public:
    GotoStatement(int line);
    void execute(EvalState &state, Program &pro);
    int getlinetarget();
};

class IfStatement : public Statement {
    private:
    Expression *lhs;
    Expression *rhs;
    char op;
    int target;
    public:
    IfStatement(Expression *left, Expression *right, char s, int targetline);
    ~IfStatement();
    void execute(EvalState &state, Program &pro);
    bool condition(EvalState &state, Program &pro);
    int get_target_line();
};

class RunStatement : public Statement {
    public:
    RunStatement();
    void execute(EvalState &state, Program &pro);
};

class ListStatement : public Statement {
    public:
    ListStatement();
    void execute(EvalState &state, Program &pro);
};

class QuitStatement : public Statement {
    public:
    QuitStatement();
    void execute(EvalState &state, Program &pro);
};

class ClearStatement : public Statement {
    public:
    ClearStatement();
    void execute(EvalState &state, Program &pro);
};

class HelpStatement : public Statement {
    public:
    HelpStatement();
    void execute(EvalState &state, Program &pro);
};


#endif
