#include "calculator.h"

double Calculator::process(const std::string& input) {
	auto tokens = tokenize(input);
	double result = process_tokens(tokens);
	for (auto ptr : tokens) {
		delete ptr;
	}
	return result;
}

std::vector<Token*> Calculator::tokenize(const std::string& input) {
	size_t curr_token_start = 0;
	std::vector<Token*> result;
	for (size_t i = 0; i < input.size(); ++i) {
		char c = input[i];
		if ('0' <= c && c <= '9' || 'a' <= c && c <= 'z' || c == '.') {
			continue;
		}

		if (curr_token_start != i) {
			result.push_back(get_fun_or_num(input.substr(curr_token_start, i - curr_token_start)));
		}

		if (c == '(' || c == ')') {
			result.push_back(new BracketToken(c));
		}
		else if (c == '+' || c == '-' || c == '*' || c == '=') {
			result.push_back(new OpToken(c));
		}
		else if (c == ',') {
			result.push_back(new CommaToken());
		}
		else if (c != ' ' && c != '\t') {
			throw std::exception(("Unkexpected character: " + std::string(1, c)).c_str());
		}

		curr_token_start = i + 1;
	}
	if (curr_token_start != input.size()) {
		result.push_back(get_fun_or_num(input.substr(curr_token_start, input.size() - curr_token_start)));
	}

	return result;
}

double Calculator::process_tokens(const std::vector<Token*> tokens) {
	auto parsed = process_sum(tokens, 0);
	if (parsed.second != tokens.size()) {
		throw std::exception("Unable to parse");
	}
	return parsed.first;
}

std::pair<double, size_t> Calculator::process_sum(const std::vector<Token*> tokens, size_t start) {
	double result = 0;
	bool plus = true;
	while (start < tokens.size()) {
		auto parsed = process_mul(tokens, start);
		if (plus) {
			result += parsed.first;
		}
		else {
			result -= parsed.first;
		}
		start = parsed.second;
		if (start == tokens.size()) {
			break;
		}
		if (tokens[start]->type() != Token::TokenType::OP) {
			return std::pair(result, start);
		}
		else {
			auto op_type = dynamic_cast<OpToken*>(tokens[start])->op_type;
			if (op_type != OpToken::OpType::ADD && op_type != OpToken::OpType::SUB) {
				return std::pair(result, start);
			}
			else {
				plus = op_type == OpToken::OpType::ADD;
				++start;
			}
		}
	}
	return std::pair(result, start);
}

std::pair<double, size_t> Calculator::process_mul(const std::vector<Token*> tokens, size_t start) {
	double result = 1;
	bool mul = true;
	while (start < tokens.size()) {
		auto parsed = process_low(tokens, start);
		if (mul) {
			result *= parsed.first;
		}
		else {
			result /= parsed.first;
		}
		start = parsed.second;
		if (start == tokens.size()) {
			break;
		}
		if (tokens[start]->type() != Token::TokenType::OP) {
			return std::pair(result, start);
		}
		else {
			auto op_type = dynamic_cast<OpToken*>(tokens[start])->op_type;
			if (op_type != OpToken::OpType::MUL && op_type != OpToken::OpType::DIV) {
				return std::pair(result, start);
			}
			else {
				mul = op_type == OpToken::OpType::MUL;
				++start;
			}
		}
	}
	return std::pair(result, start);
}

std::pair<double, size_t> Calculator::process_brackets(const std::vector<Token*> tokens, size_t start) {
	if (tokens[start]->type() != Token::TokenType::BRACKET ||
		dynamic_cast<BracketToken*>(tokens[start])->bracket_type != BracketToken::BracketType::OPEN) {
		throw "Expected opening bracket";
	}
	auto parsed = process_sum(tokens, start + 1);
	if (tokens[parsed.second]->type() != Token::TokenType::BRACKET ||
		dynamic_cast<BracketToken*>(tokens[parsed.second])->bracket_type != BracketToken::BracketType::CLOSE) {
		throw "Expected closing bracket";
	}
	return std::pair(parsed.first, parsed.second + 1);
}

std::pair<double, size_t> Calculator::process_fun(const std::vector<Token*> tokens, size_t start) {
	auto fun_type = dynamic_cast<FunToken*>(tokens[start])->fun_type;
	if (start + 1 == tokens.size() || tokens[start + 1]->type() != Token::TokenType::BRACKET ||
		dynamic_cast<BracketToken*>(tokens[start + 1])->bracket_type != BracketToken::BracketType::OPEN) {
		throw std::exception("Expected opening bracket");
	}
	auto lhs = process_sum(tokens, start + 2);
	if (lhs.second == tokens.size() || tokens[lhs.second]->type() != Token::TokenType::COMMA) {
		throw std::exception("Expected comma");
	}
	auto rhs = process_sum(tokens, lhs.second + 1);
	if (rhs.second == tokens.size() || tokens[rhs.second]->type() != Token::TokenType::BRACKET ||
		dynamic_cast<BracketToken*>(tokens[rhs.second])->bracket_type != BracketToken::BracketType::CLOSE) {
		throw std::exception("Expected closing bracket");
	}
	if (true) {
		return std::pair(log(lhs.first) / log(rhs.first), rhs.second + 1);
	}
	return std::pair(pow(lhs.first, rhs.first), rhs.second + 1);
}

std::pair<double, size_t> Calculator::process_low(const std::vector<Token*> tokens, size_t start) {
	switch (tokens[start]->type()) {
	case Token::TokenType::NUM:
		return std::pair(dynamic_cast<NumToken*>(tokens[start])->value, start + 1);

	case Token::TokenType::BRACKET:
		return process_brackets(tokens, start);

	case Token::TokenType::FUN: {
		return process_fun(tokens, start);
	}

	case Token::TokenType::OP: {
		auto op_type = dynamic_cast<OpToken*>(tokens[start])->op_type;
		if (op_type == OpToken::OpType::ADD) {
			return process_sum(tokens, start + 1);
		}
		if (op_type == OpToken::OpType::SUB) {
			auto parsed = process_sum(tokens, start + 1);
			return std::pair(-parsed.first, parsed.second);
		}
	}
	default:
		throw std::exception("Unable to parse");
	}

}

Token* Calculator::get_fun_or_num(const std::string& s) {
	if (is_double(s)) {
		return new NumToken(std::stod(s));
	}
	else if (s == "log" || s == "pow") {
		return new FunToken(s);
	}
	throw std::exception(("Unable to parse " + s).c_str());
}

bool Calculator::is_double(const std::string& str)
{
	char* endptr = 0;
	strtod(str.c_str(), &endptr);

	if (*endptr != '\0' || endptr == str)
		return false;
	return true;
}