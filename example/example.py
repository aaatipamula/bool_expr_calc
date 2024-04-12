#!/usr/bin/env python3

import sys
from pprint import pp
from enum import Enum
from dataclasses import dataclass
from typing import List, Optional

# BNF (Backus-Naur Form) Order of Operations
# Expr    -> OpXor
# OpXor   -> OpOr ( "$" OpOr )*
# OpNand  -> OpOr ( "@" OpOr )*
# OpOr    -> OpAnd ( "|" OpAnd )*
# OpAnd   -> Unary ( "&" Unary )*
# Unary   -> "!" Unary | Primary
# Primary -> "T" | "F" | "(" Expr ")"

# SUBSYSTEM: None
# PACKAGE: Util
class ParseError(BaseException): ...  # Something went wrong parsing


# SUBSYSTEM: Evaluation
# PACKAGE: Tokens
# TokenType: All the types of valid tokens (represented as integers)
# NOTE: In order of operations
class TokenType(Enum):
    UNDEF   = float('inf')
    NOT     = 5
    AND     = 4
    OR      = 3
    NAND    = 2
    XOR     = 1
    VAL     = 0
    LPAREN  = -1
    RPAREN  = -2


# SUBSYSTEM: Evaluation
# PACKAGE: Tokens
# Token: Contain useful data about a token
@dataclass
class Token:
    type: TokenType # Type of token (programmatically)
    pos: int        # Character position
    lexeme: str     # The literal character
    value: Optional[bool] = None # The literal value (T or F)

    # Operator overloads for Tokens, TokenType, and int for (==)
    def __eq__(self, __value: object) -> bool:
        if isinstance(__value, Token):
            return self.type == __value.type
        elif isinstance(__value, int) or isinstance(__value, TokenType):
            return self.type == __value
        raise TypeError(f"Invalid type comparison with Token and {type(__value)}")

    # Operator overloads for Tokens, TokenType, and int for (!=)
    def __ne__(self, __value: object) -> bool:
        if isinstance(__value, Token):
            return self.type != __value.type
        elif isinstance(__value, int) or isinstance(__value, TokenType):
            return self.type != __value
        raise TypeError(f"Invalid type comparison with Token and {type(__value)}")


# SUBSYSTEM: Evaluation
# PACKAGE: Tokens
# Tokenizer: Turn a character string into a list of objects with useful information.
# NOTE: Uses an enum TokenType and the Token dataclass to create a list of useful tokens

def tokenize(line: str) -> List[Token]:
    tokens = []
    for pos, char in enumerate(line, 1):
        match char:
            case '(': tokens.append(Token(TokenType.LPAREN, pos, char))
            case ')': tokens.append(Token(TokenType.RPAREN, pos, char))
            case '!': tokens.append(Token(TokenType.NOT, pos, char))
            case '&': tokens.append(Token(TokenType.AND, pos, char))
            case '|': tokens.append(Token(TokenType.OR, pos, char))
            case '@': tokens.append(Token(TokenType.NAND, pos, char))
            case '$': tokens.append(Token(TokenType.XOR, pos, char))
            case 'T': tokens.append(Token(TokenType.VAL, pos, char, True))
            case 'F': tokens.append(Token(TokenType.VAL, pos, char, False))
            case '\t' | '\r' | '\n' | ' ':
                continue
            case _:
                raise SyntaxError("Invalid token '" + char + "' found.")
    return tokens


# SUBSYSTEM: Evaluation
# PACKAGE: Ast
# Abstract Syntax Tree (AST) Nodes: Building blocks for the AST

# Expr: Any expression (abstract base class)
class Expr: ...

# Literal: Any literal value (T or F)
@dataclass
class Literal(Expr):
    value: bool     # Only contains one value

# Binary: A binary operation (&, |, @, $)
@dataclass
class Binary(Expr):
    left: Expr      # Left expression
    op: TokenType   # Operator
    right: Expr     # Right expression

# Unary: A prefix operation (!)
@dataclass
class Unary(Expr):
    op: TokenType   # Operator (should only be !)
    right: Expr     # Right expression

# Group: A grouped expression
@dataclass
class Group(Expr):
    expression: Expr # The grouped expression


# SUBSYSTEM: Evaluation
# PACKAGE: Parser
# Parse: Use recursive descent to parse the token list
# NOTE: Everything parser related should be contained in a parser class
#       This is marked with "# PAR" above the function definition line

# Pointer to current token
# PAR
global current # self.current in parser class
current = 0

# Get the previous token
# PAR
def prev_tkn(tokens: List[Token]) -> Token:
    global current
    if current <= len(tokens):
        return tokens[current - 1]
    return TokenType.UNDEF # Known type error just work with me please

# Get the current token
# PAR
def curr_tkn(tokens: List[Token]) -> Token:
    global current
    if current < len(tokens):
        return tokens[current]
    return TokenType.UNDEF # Known type error just work with me its late 

# Parse the primary token types (T, F, '(', ')')
# PAR
def primary(tokens: List[Token]) -> Expr:
    global current
    tkn = curr_tkn(tokens)
    # Parse a VAL as just a Literal
    if tkn == TokenType.VAL:  
        current += 1
        return Literal(tkn.value) # Yes I know there is a type error.

    # Parse a PAREN
    if tkn == TokenType.LPAREN:
        current += 1
        # Parse the expression within the group
        expr = xor(tokens)
        # Raise an error if the expression is not complete
        if curr_tkn(tokens).type != TokenType.RPAREN:
            current = 0
            raise SyntaxError("Missing right paren.")
        current += 1
        return Group(expr)

    current = 0
    err_tkn = curr_tkn(tokens)
    err = f"Found an unexpected token '{err_tkn.lexeme}'"
    raise SyntaxError(err)

# Highest precidence operation
# PAR
def unary(tokens: List[Token]) -> Expr:
    # If there is a prefix add a Unary and parse the rest
    if curr_tkn(tokens) == TokenType.NOT:
        global current
        current += 1
        op = prev_tkn(tokens).type
        right = unary(tokens)
        return Unary(op, right)
    
    # Otherwise parse the primary token types
    return primary(tokens)

# Second highest precidence
# PAR
def _and(tokens: List[Token]) -> Expr:
    expr = unary(tokens) # Parse a unary expression
   
    # Continue parsing the and operator while we find it
    while curr_tkn(tokens) == TokenType.AND:
        global current
        current += 1
        op = prev_tkn(tokens).type
        right = unary(tokens)
        expr = Binary(expr, op, right)
    
    # Return the final expression
    return expr

# Third highest precidence
# PAR
def _or(tokens: List[Token]) -> Expr:
    expr = _and(tokens) # Parse a and expression
   
    # Continue parsing the or operator while we find it
    while curr_tkn(tokens) == TokenType.OR:
        global current
        current += 1
        op = prev_tkn(tokens).type
        right = _and(tokens)
        expr = Binary(expr, op, right)
    
    # Return the final expression
    return expr

# Fourth highest precidence
# PAR
def nand(tokens: List[Token]) -> Expr:
    expr = _or(tokens)
   
    while curr_tkn(tokens) == TokenType.NAND:
        global current
        current += 1
        op = prev_tkn(tokens).type
        right = _or(tokens)
        expr = Binary(expr, op, right)
    
    return expr

# Fifth highest precidence
# PAR
def xor(tokens: List[Token]) -> Expr:
    expr = nand(tokens)
   
    while curr_tkn(tokens) == TokenType.XOR:
        global current
        current += 1
        op = prev_tkn(tokens).type
        right = nand(tokens)
        expr = Binary(expr, op, right)
    
    return expr

# Parse from the top level
# PAR
def parse(tokens: List[Token], line: str) -> Expr:
    global current
    ast = xor(tokens) # Pars the ast out
    if current != len(tokens): # If there are leftover tokens there is an error
        err_tkn = tokens[current]
        err = f"Error at token '{err_tkn.lexeme}': "
        err += f"{line}\n{'^'.rjust(len(err) + err_tkn.pos)}"
        current = 0
        raise SyntaxError(err)
    current = 0
    return ast


# SUBSYSTEM: Evaluation
# PACKAGE: Interpret
# Eval: Use recursive descent to parse the token list
# NOTE: Everything evaluation related should be grouped in an Eval class
#       This is marked with "# EVL" before a function definition

# Evaluate a unary expression in the AST
# EVL
def evalUnary(node: Unary) -> bool:
    right = eval(node.right)
    return not right

# Evaluate a binary expression in the AST
# EVL
def evalBinary(node: Binary) -> bool:
    left = eval(node.left)
    right = eval(node.right)

    # Match the operation 
    match node.op:
        case TokenType.AND: return right and left
        case TokenType.OR: return right or left
        case TokenType.XOR: return right != left
        case TokenType.NAND: return not(left and right)

    # No valid operand found
    raise RuntimeError("Invalid operand found")

# Evaluate an AST
# EVL
def eval(node: Expr) -> bool:
    if isinstance(node, Literal): return node.value
    if isinstance(node, Binary): return evalBinary(node)
    if isinstance(node, Unary): return evalUnary(node)
    if isinstance(node, Group): return eval(node.expression)

    raise RuntimeError("Inavlid AST Node found")


# SUBSYSTEM: Input/Output
# PACKAGE: IO
# IO: Input and output to screen
# NOTE: Everything input/output related should be grouped in an IO class
#       This is marked with "# IO" before a function definition

# Check if a valid escape string is entered
# IO
def check_exit(line: str) -> bool:
    line = line.strip().lower()
    return line in ("quit", "q", "exit")


# Run the main program
# IO
def main():
    DEBUG = False

    # Check for debug arguments
    if len(sys.argv) > 1:
        DEBUG = sys.argv[1].lower() == "debug"

    # Run an REPL (Read Evaluate Print Loop)
    while True:
        try: 
            # READ user input and check for exits
            line = input(">> ")
            if check_exit(line): 
                print("Good Bye!")
                exit(0)

            # Tokenize everything into a list
            tkns = tokenize(line)
            # Parse everything into a tree
            ast = parse(tkns, line)

            # Print the list and tree if specified
            if DEBUG:
                pp(tkns)
                print()
                pp(ast)

            # EVAL and PRINT the result
            final = eval(ast)
            print(final)

        # Something went wrong with the user input
        except SyntaxError as err:
            print("Something was wrong with your input.")
            print(err)

        # Something went wrong parsing the user input
        except ParseError as err:
            print("Something went wrong parsing your input")
            print(err)

        # Something went wrong evaluating
        except RuntimeError as err:
            print("Something went wrong evaluating your input")
            print(err)

        # User wants to quit using the program
        except KeyboardInterrupt:
            print("\nGood Bye!")
            exit(0)

        # Something went wrong in general
        except Exception as err:
            print("Panic! (At the Disco)")
            raise err


if __name__ == "__main__":
    main()

