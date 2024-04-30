#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Tokens.h"

/* enum NodeType { */
/*   BINARY, UNARY, GROUPING, LITERAL */
/* }; */

/* class Expr { */
/*   public: */
/*     NodeType type; */
/*     TokenType op; */
/*     Expr *left; */
/*     Expr *right; */
/*     bool value; */
/*     Expr *expr; */

/*     virtual ~Expr() = default; */

/*     Expr( */
/*       NodeType type, */
/*       TokenType op = UNDEF, */
/*       Expr *left = nullptr, */
/*       Expr *right = nullptr, */
/*       bool value = false, */
/*       Expr *expr = nullptr */
/*     ) */
/*       : type(type), op(op), left(left), right(right), value(value), expr(expr) {} */
/* }; */

/* class Binary : public Expr { */
/*   public: */
/*     Expr &left; */
/*     Token op; */
/*     Expr &right; */
/*     NodeType type; */
/*     Binary(Expr &left, Token op, Expr &right, NodeType type) */
/*       : left(left), op(op), right(right), type(type) {} */
/* }; */

/* class Unary : public Expr { */
/*   public: */
/*     Token op; */
/*     Expr &right; */
/*     NodeType type; */
/*     Unary(Token op, Expr &right, NodeType type) */
/*       : op(op), right(right), type(type) {} */
/* }; */

/* class Grouping : public Expr { */
/*   public: */
/*     Expr &expression; */
/*     NodeType type; */
/*     Grouping(Expr &expression, NodeType type) */
/*       : expression(expression), type(type) {} */
/* }; */

/* class Literal : public Expr { */
/*   public: */
/*     bool value; */
/*     NodeType type; */
/*     Literal(bool value, NodeType type) */
/*       : value(value), type(type) {} */
/* }; */

/**
 * Associated parsing functionns
 */
class Parser {
  int curr_pos = 0;

  /**
   * Match the current token with the type and advance if matched
   */
  bool match(TokenType type);
  Token curr_tkn();
  Token prev_tkn();

  bool parseXor();
  bool parseNand();
  bool parseOr();
  bool parseAnd();
  bool parseNot();
  bool primary();

  public:
    std::vector<Token> tokens;
    bool parse();
};

#endif
