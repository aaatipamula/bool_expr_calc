#include "Interpreter.h"

bool Interpreter::evalBinary(Expr *expression) {
	switch (expression->op) {
		case AND:
			return interpret(expression->left) && interpret(expression->right);
			break;
		case OR:
			return interpret(expression->left) || interpret(expression->right);
			break;
		case XOR:
			return interpret(expression->left) != interpret(expression->right);
			break;
		case NAND:
			return not(interpret(expression->left) && interpret(expression->right));
			break;
		default:
			throw "Invalid binary expression.";
	}	
}

bool Interpreter::interpret(Expr *expression) {
	switch (expression->type) {
		case LITERAL:
			return expression->value;
			break;
		case UNARY:
			return not(interpret(expression->left));
			break;
		case GROUPING:
			return interpret(expression->expr);
			break;
		case BINARY:
			return evalBinary(expression);
			break;
		default:
			throw "Invalid node found when evaluating";
	}
}

