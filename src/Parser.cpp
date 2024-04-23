#include "Parser.h"

Token Parser::curr_tkn() {
  return tokens.at(curr_pos);
}

Token Parser::prev_tkn() {
  return tokens.at(curr_pos - 1);
}

bool Parser::match(TokenType type) {
  if (curr_tkn().type == type) {
    curr_pos++;
    return true;
  }
  return false;
}

Expr Parser::parseXor() {
  Expr expr = parseNand();
  while (match(XOR)) {
    Token op = prev_tkn();
    Expr _right = parseNand();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseNand() {
  Expr expr = parseOr();
  while (match(NAND)) {
    Token op = prev_tkn();
    Expr _right = parseOr();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseOr() {
  Expr expr = parseAnd();
  while (match(OR)) {
    Token op = prev_tkn();
    Expr _right = parseAnd();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseAnd() {
  Expr expr = parseNot();
  while (match(AND)) {
    Token op = prev_tkn();
    Expr _right = parseNot();
    expr = Binary(expr, op, _right);
  }
  return expr;
}

Expr Parser::parseNot() {
  if (match(NOT)) {
    Token op = prev_tkn();
    Expr _right = parseAnd();
    return Unary(op, _right);
  }
  return primary();
}

Expr Parser::primary() {
  if (match(VAL)) {
    bool value = curr_tkn().value;
    return Literal(value);
  }

  if (match(LPAREN)) {
    Expr expr = parseXor();
    TokenType curr = curr_tkn().type;
    if (curr != RPAREN) {
      throw "Missing matching parenthesis.";
    }
    curr_pos++;
    return Grouping(expr);
  }

  throw "Invalid expression found.";
}

