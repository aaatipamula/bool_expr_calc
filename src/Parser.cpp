#include "Parser.h"
#include <iostream>

Token Parser::curr_tkn() {
  return tokens.at(curr_pos);
}

Token Parser::prev_tkn() {
  return tokens.at(curr_pos - 1);
}

bool Parser::match(TokenType type) {
  // Invalid range return false
  if (curr_pos >= tokens.size()) return false;
  // Match type and consume if matched
  if (curr_tkn() == type) {
    curr_pos++;
    return true;
  }
  return false;
}

bool Parser::parseXor() {
  // Parse for XOR first
  bool expr = parseNand();
  // then contine parsing for NAND
  while (match(XOR)) {
    bool _right = parseNand();
    expr = expr != _right;
  }
  return expr;
}

bool Parser::parseNand() {
  // Parse for OR first
  bool expr = parseOr();
  // Then continue parsing for XOR
  while (match(NAND)) {
    bool _right = parseOr();
    expr = not(expr && _right);
  }
  return expr;
}

bool Parser::parseOr() {
  // Parse for AND first
  bool expr = parseAnd();
  // Then continue parsing for OR
  while (match(OR)) {
    bool _right = parseAnd();
    expr = expr || _right;
  }
  return expr;
}

bool Parser::parseAnd() {
  // Parse for unary operator first
  bool expr = parseNot();
  // Then continue parsing for AND
  while (match(AND)) {
    bool _right = parseNot();
    expr = expr && _right;
  }
  return expr;
}

bool Parser::parseNot() {
  // Parse for a unary operator if exists
  if (match(NOT)) {
    bool _right = parseNot();
    return not(_right);
  }
  // Then parse for a primary
  return primary();
}

bool Parser::primary() {
  // Match for a literal value
  if (match(VAL)) {
    return prev_tkn().value;
  }

  // Match for grouped expression
  if (match(LPAREN)) {
    bool expr = parseXor();
    TokenType curr = curr_tkn().type;
    if (curr != RPAREN) {
      throw "Missing matching parenthesis.";
    }
    curr_pos++;
    return expr;
  }

  // If no token was parsed we have an invalid expression
  throw "Invalid expression found.";
}

bool Parser::parse() {
  bool expr = parseXor();
  // Parsing failed if we didn't reach the end of the tokens
  if (curr_pos != tokens.size()) {
    throw "Invalid token sequence.";
  }
  return expr;
}

