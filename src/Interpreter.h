#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"

// Interpreter code here

class Interpreter {
  bool evalBinary(Expr *expression);
  public:
    bool interpret(Expr *expression);
};

#endif
