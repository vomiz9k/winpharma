#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include <string_view>
#include <cassert>

class Token {
public:
	enum class TokenType {
		OP,
		NUM,
		BRACKET,
		FUN,
		COMMA
	};
	virtual TokenType type() = 0;
};

class NumToken : public Token {
public:
	double value;
	NumToken(double value) : value(value) {}
	TokenType type() override {
		return TokenType::NUM;
	}
};

class OpToken : public Token {
public:
	enum class OpType {
		ADD,
		SUB,
		MUL,
		DIV
	};

	OpType op_type = OpType::ADD;

	OpToken(char op) {
		switch (op) {
		case '+':
			op_type = OpType::ADD;
			break;

		case '-':
			op_type = OpType::SUB;
			break;

		case '*':
			op_type = OpType::MUL;
			break;

		case '/':
			op_type = OpType::DIV;
			break;
		}
	}

	TokenType type() override {
		return TokenType::OP;
	}
};

class FunToken : public Token {
public:
	enum class FunType {
		LOG,
		POW
	};

	FunType fun_type;
	FunToken(const std::string& name) {
		if (name == "log") {
			fun_type = FunType::LOG;
		}
		else {
			fun_type = FunType::POW;
		}
	}

	TokenType type() override {
		return TokenType::FUN;
	}
};

class BracketToken : public Token {
public:
	enum class BracketType {
		OPEN,
		CLOSE
	};

	BracketType bracket_type;
	BracketToken(char c) {
		if (c == '(') {
			bracket_type = BracketType::OPEN;
		}
		else {
			bracket_type = BracketType::CLOSE;
		}
	}

	TokenType type() override {
		return TokenType::BRACKET;
	}
};

class CommaToken : public Token {
public:
	TokenType type() override {
		return TokenType::COMMA;
	}
};


class Calculator {
public:
	double process(const std::string& input);
	std::vector<Token*> tokenize(const std::string& input);

	double process_tokens(const std::vector<Token*> tokens);

	std::pair<double, size_t> process_sum(const std::vector<Token*> tokens, size_t start);

	std::pair<double, size_t> process_mul(const std::vector<Token*> tokens, size_t start);

	std::pair<double, size_t> process_brackets(const std::vector<Token*> tokens, size_t start);

	std::pair<double, size_t> process_fun(const std::vector<Token*> tokens, size_t start);

	std::pair<double, size_t> process_low(const std::vector<Token*> tokens, size_t start);

	Token* get_fun_or_num(const std::string& s);

	static bool is_double(const std::string& str);
};
