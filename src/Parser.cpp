#include "Parser.h"
#include <iostream>

Token Parser::curr_tkn() {
  return tokens.at(curr_pos);
}

Token Parser::prev_tkn() {
  return tokens.at(curr_pos - 1);
}

bool Parser::match(TokenType type) {
  if (curr_pos >= tokens.size()) return false;
  if (curr_tkn() == type) {
    curr_pos++;
    return true;
  }
  return false;
}

bool Parser::parseXor() {
  bool expr = parseNand();
  while (match(XOR)) {
    Token op = prev_tkn();
    bool _right = parseNand();
    expr = expr != _right;
  }
  return expr;
}

bool Parser::parseNand() {
  bool expr = parseOr();
  while (match(NAND)) {
    Token op = prev_tkn();
    bool _right = parseOr();
    expr = not(expr && _right);
  }
  return expr;
}

bool Parser::parseOr() {
  bool expr = parseAnd();
  while (match(OR)) {
    Token op = prev_tkn();
    bool _right = parseAnd();
    expr = expr || _right;
  }
  return expr;
}

bool Parser::parseAnd() {
  bool expr = parseNot();
  while (match(AND)) {
    Token op = prev_tkn();
    bool _right = parseNot();
    expr = expr && _right;
  }
  return expr;
}

bool Parser::parseNot() {
  if (match(NOT)) {
    Token op = prev_tkn();
    bool _right = parseAnd();
    return not(_right);
  }
  return primary();
}

bool Parser::primary() {
  if (match(VAL)) {
    return prev_tkn().value;
  }

  if (match(LPAREN)) {
    bool expr = parseXor();
    TokenType curr = curr_tkn().type;
    if (curr != RPAREN) {
      throw "Missing matching parenthesis.";
    }
    curr_pos++;
    return expr;
  }

  throw "Invalid expression found.";
}

bool Parser::parse() {
  bool expr = parseXor();
  if (curr_pos != tokens.size()) {
    throw "Invalid token sequence.";
  }
  return expr;
}

