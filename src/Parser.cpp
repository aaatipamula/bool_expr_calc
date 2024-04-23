#include <list>
#include "Parser.h"

Expr Parser::parseXor() {
  Expr expr = parseNand();
  while (match(/**/)) {
    Token op = prev_tkn();
    Expr _right = parseNand();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseNand() {
  Expr expr = parseOr();
  while (match(/**/)) {
    Token op = prev_tkn();
    Expr _right = parseOr();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseOr() {
  Expr expr = parseAnd();
  while (match(/**/)) {
    Token op = prev_tkn();
    Expr _right = parseAnd();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseAnd() {
  Expr expr = parseNot();
  while (match(/**/)) {
    Token op = prev_tkn();
    Expr _right = parseNot();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseNot() {
  if (match(/**/)) {
    Token op = prev_tkn();
    Expr _right = parseAnd();
    return Unary(op, _right);
  }
  return primary();
}

Expr Parser::primary() {
  if (match(/**/)) {
    bool value = curr_tkn().value;
    return Literal(value);
  }

  if (match(/**/)) {
    expr = parseXor();
    TokenType curr = curr_tkn().type;
    if (curr != RPAREN) {
      throw "Some error"
    }
    curr_pos++;
    return Group(expr)
  }
}

