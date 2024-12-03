cmake_minimum_required(VERSION 3.22) # check your cmake version and modify this line

set(CMAKE_CXX_STANDARD 20) # c++20 is required
project(Calculator)

include_directories(/usr/include/antlr4-runtime) # including runtime package

include_directories(src)

add_executable(code
        src/ExprLexer.h
        src/ExprLexer.cpp
        src/ExprParser.h
        src/ExprParser.cpp
        src/ExprVisitor.h
        src/ExprVisitor.cpp
        EvalVisitor.h
        main.cpp
)

target_link_libraries(code antlr4-runtime)