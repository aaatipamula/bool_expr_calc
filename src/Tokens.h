#ifndef TOKENS_H
#define TOKENS_H

// Better than array
#include <vector>
#include <string> // Known clang LSP error

// Enumerate all the types of characters we encounter
enum TokenType {
  LPAREN, RPAREN, VAL, AND,
  OR, XOR, NAND, NOT, UNDEF
};

class Token {
  public:
    // For evalutation and parsing
    TokenType type;
    bool value;

    // Operator overloads for the equality operator (==)
    bool operator == (const TokenType rhs);

    // Token class constructor
    Token(TokenType type, bool value=false)
      : type(type), value(value) {}
};

/**
 * Convert a line into a vector (array) of tokens
 * NOTE: No constructor needed for this
 */
class Tokenizer {
  public:
    std::vector<Token> tokenize(std::string line);
};

#endif

