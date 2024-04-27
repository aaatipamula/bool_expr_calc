#include "Tokens.h"

bool Token::operator==(const TokenType rhs) {
  return rhs == type;
}

std::vector<Token> Tokenizer::tokenize(std::string line) {

  std::vector<Token> tokens;

  for (int i = 0; i < line.length(); i ++) {
    switch(line[i]) {
      case '(':
        tokens.push_back(Token(LPAREN));
        break;
      case ')':
        tokens.push_back(Token(RPAREN));
        break;
      case '!':
        tokens.push_back(Token(NOT));
        break;
      case '&':
        tokens.push_back(Token(AND));
        break;
      case '|':
        tokens.push_back(Token(OR));
        break;
      case '@':
        tokens.push_back(Token(NAND));
        break;
      case '$':
        tokens.push_back(Token(XOR));
        break;
      case 'T':
        tokens.push_back(Token(VAL, true));
        break;
      case 'F':
        tokens.push_back(Token(VAL, false));
        break;
      case '\t':
      case'\r':
      case '\n':
      case ' ':
        break;
      default:
        throw "Invalid character found";
    }
  }

  return tokens;
}

