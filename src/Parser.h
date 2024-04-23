#ifndef EXPR_H
#define EXPR_H

#include <any>
#include <memory>
#include <utility>
#include <list>
#include "Tokens.h"

class Expr {
  public:
    virtual ~Expr() = default;
};

class Binary : public Expr {
  std::unique_ptr<Expr> left;
  Token op;
  std::unique_ptr<Expr> right;
  public:
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
      : left(std::move(left)), op(op), right(std::move(right)) {}
};

class Unary : public Expr {
  Token op;
  std::unique_ptr<Expr> right;
  public:
    Unary(Token op, std::unique_ptr<Expr> right)
      : op(op), right(std::move(right)) {}
};

class Grouping : public Expr {
  std::unique_ptr<Expr> expression;
  public:
  Grouping(std::unique_ptr<Expr> expression)
    : expression(std::move(expression)) {}
};

class Literal : public Expr {
  bool value;
  public:
  Literal(bool value)
    : value(value) {}
};

class Parser {
  int curr_pos = 0;
  std::list<Token> tokens;

  bool match();
  Token curr_tkn();
  Token prev_tkn();

  Expr parseXor();
  Expr parseNand();
  Expr parseOr();
  Expr parseAnd();
  Expr parseNot();
  Expr primary();

  public:
    Parser(std::list<Token> tokens)
      : tokens(tokens) {}

    Expr parse();
};

#endif
