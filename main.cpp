#include <iostream>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Calculator {
protected:
    std::stack<double> values;
    std::stack<char> ops;
    std::vector<std::string> steps;

    int precedence(char op) const {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    double applyOp(double a, double b, char op) const {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw std::invalid_argument("Ошибка деления на 0");
                return a / b;
        }
        return 0;
    }

    void processOp() {
        if (values.size() < 2 || ops.empty()) throw std::invalid_argument("Неверное выражение");

        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        double result = applyOp(val1, val2, op);
        values.push(result);

        std::ostringstream oss;
        oss << val1 << " " << op << " " << val2 << " = " << result;
        steps.push_back(oss.str());
    }

public:
    double evaluate(const std::string &expression) {
        std::istringstream iss(expression);
        char token;

        while (iss >> token) {
            if (isdigit(token) || token == '.') {
                iss.putback(token);
                double value;
                if (!(iss >> value)) {
                    throw std::invalid_argument("Неверный ввод числа");
                }
                values.push(value);
            } else if (token == '(') {
                ops.push(token);
            } else if (token == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    processOp();
                }
                if (ops.empty()) {
                    throw std::invalid_argument("Неправильно расставлены скобки");
                }
                ops.pop();
            } else if (token == '+' || token == '-' || token == '*' || token == '/') {
                while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
                    processOp();
                }
                ops.push(token);
            } else {
                throw std::invalid_argument("Неверный символ");
            }
        }

        while (!ops.empty()) {
            processOp();
        }

        if (values.size() != 1 || !iss.eof()) {
            throw std::invalid_argument("Неправильное выражение");
        }

        return values.top();
    }

    void printSteps() const {
        std::cout << "\nШаги вычисления:" << std::endl;
        for (const auto &step : steps) {
            std::cout << step << std::endl;
        }
    }
    void clearSteps() {
        steps.clear();
    }
};

int main() {
    char mode;
    Calculator calc;

    std::string expression;
    std::cout << "Введите выражение: ";
    std::getline(std::cin, expression);

    try {
        std::cout << "Выберите режим (упрощенный - u, расширенный - r): ";
        std::cin >> mode;
        std::cin.ignore(); 

        if (mode != 'u' && mode != 'r') {
            std::cout << "Неверный режим." << std::endl;
            return 1;
        }

        double result = calc.evaluate(expression);
        if (mode == 'r') {
            calc.printSteps(); 
        }
        std::cout << "\nРезультат: " << result << "\n" << std::endl;

    } catch (const std::invalid_argument &e) {
        std::cout << "\nОшибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
