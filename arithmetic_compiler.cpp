//COMPILER DESIGN BASICS
#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

class Parser {
    const std::string& input;
    size_t pos;

    char peek() const {
        while (pos < input.size() && isspace(input[pos])) ++pos;
        return (pos < input.size()) ? input[pos] : '\0';
    }

    char get() {
        char c = peek();
        if (pos < input.size()) ++pos;
        return c;
    }

    // Grammar:
    // expr   = term   { ('+' | '-') term }
    // term   = factor { ('*' | '/') factor }
    // factor = NUMBER | '(' expr ')'

    double expr() {
        double result = term();
        while (true) {
            char op = peek();
            if (op == '+') {
                get();
                result += term();
            } else if (op == '-') {
                get();
                result -= term();
            } else {
                break;
            }
        }
        return result;
    }

    double term() {
        double result = factor();
        while (true) {
            char op = peek();
            if (op == '*') {
                get();
                result *= factor();
            } else if (op == '/') {
                get();
                double divisor = factor();
                if (divisor == 0) throw std::runtime_error("Division by zero");
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    double factor() {
        char c = peek();
        if (c == '(') {
            get(); // consume '('
            double val = expr();
            if (get() != ')') throw std::runtime_error("Expected ')'");
            return val;
        } else if (isdigit(c) || c == '.') {
            return number();
        } else if (c == '-') {
            get();
            return -factor();
        } else {
            throw std::runtime_error("Unexpected character");
        }
    }

    double number() {
        size_t start = pos;
        while (isdigit(peek()) || peek() == '.') get();
        if (start == pos) throw std::runtime_error("Expected number");
        return std::stod(input.substr(start, pos - start));
    }

public:
    Parser(const std::string& in) : input(in), pos(0) {}

    double parse() {
        double value = expr();
        if (peek() != '\0') throw std::runtime_error("Unexpected input after expression");
        return value;
    }
};

int main() {
    std::cout << "Simple Arithmetic Expression Compiler/Evaluator\n";
    std::cout << "Type 'exit' to quit.\n";
    while (true) {
        std::cout << "\nEnter expression: ";
        std::string line;
        std::getline(std::cin, line);
        if (line == "exit") break;
        try {
            Parser parser(line);
            double result = parser.parse();
            std::cout << "Result: " << result << '\n';
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << '\n';
        }
    }
    return 0;
}
