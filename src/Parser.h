#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Tokens.h"

class Expr {
  public:
    virtual ~Expr() = default;
};

class Binary : public Expr {
  Expr &left;
  Token op;
  Expr &right;
  public:
    Binary(Expr &left, Token op, Expr &right)
      : left(left), op(op), right(right) {}
};

class Unary : public Expr {
  Token op;
  Expr &right;
  public:
    Unary(Token op, Expr &right)
      : op(op), right(right) {}
};

class Grouping : public Expr {
  Expr &expression;
  public:
    Grouping(Expr &expression)
      : expression(expression) {}
};

class Literal : public Expr {
  bool value;
  public:
    Literal(bool value)
      : value(value) {}
};

class Parser {
  int curr_pos = 0;
  std::vector<Token> tokens;

  bool match(TokenType type);
  Token curr_tkn();
  Token prev_tkn();

  Expr parseXor();
  Expr parseNand();
  Expr parseOr();
  Expr parseAnd();
  Expr parseNot();
  Expr primary();

  public:
    Parser(std::vector<Token> tokens)
      : tokens(tokens) {}

    Expr parse();
};

#endif
