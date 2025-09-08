#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

// Function to set precedence of operators
int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Check if character is operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

// Convert Infix to Postfix
string infixToPostfix(string infix) {
    stack<char> st;
    string postfix = "";

    for (size_t i = 0; i < infix.length(); i++) {
        char c = infix[i];

        // If operand (number or variable)
        if (isdigit(c)) {
            // Handle multi-digit numbers
            while (i < infix.length() && isdigit(infix[i])) {
                postfix += infix[i];
                i++;
            }
            postfix += ' '; // space to separate numbers
            i--;
        }
        else if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                postfix += ' ';
                st.pop();
            }
            if (!st.empty()) st.pop(); // remove '('
        }
        else if (isOperator(c)) {
            while (!st.empty() && precedence(st.top()) >= precedence(c)) {
                postfix += st.top();
                postfix += ' ';
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop remaining operators
    while (!st.empty()) {
        postfix += st.top();
        postfix += ' ';
        st.pop();
    }

    return postfix;
}

// Evaluate Postfix Expression
double evaluatePostfix(string postfix) {
    stack<double> st;
    string num = "";

    for (size_t i = 0; i < postfix.length(); i++) {
        char c = postfix[i];

        if (isdigit(c)) {
            num = "";
            while (i < postfix.length() && isdigit(postfix[i])) {
                num += postfix[i];
                i++;
            }
            st.push(stod(num));
            i--; // step back
        }
        else if (isOperator(c)) {
            double val2 = st.top(); st.pop();
            double val1 = st.top(); st.pop();

            switch (c) {
                case '+': st.push(val1 + val2); break;
                case '-': st.push(val1 - val2); break;
                case '*': st.push(val1 * val2); break;
                case '/': st.push(val1 / val2); break;
                case '^': st.push(pow(val1, val2)); break;
            }
        }
    }

    return st.top();
}

int main() {
    string infix;
    cout << "Enter an infix expression (e.g., (3+5)*2): ";
    getline(cin, infix);

    string postfix = infixToPostfix(infix);
    cout << "Postfix Expression: " << postfix << endl;

    double result = evaluatePostfix(postfix);
    cout << "Result: " << result << endl;

    return 0;
}
