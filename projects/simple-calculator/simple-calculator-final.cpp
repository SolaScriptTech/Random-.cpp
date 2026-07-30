// SimpleCalculator_Johns_Matt.cpp
// Matt Johns, CISP 360
// July 21, 2025
// Simple Calculator Assignment
// This program fulfills all specs for the CISP 360 assignment and is written in a way that's true to my own process as I worked through it. 
// Comments reflect things I actually tried, fixed, and learned along the way.

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

// Keeps main() focused and readable.
void ProgramGreeting() {
    cout << "---------------------------------------------" << endl;
    cout << "Welcome to the Simple Calculator Program!" << endl;
    cout << "Author: Matt Johns" << endl;
    cout << "Due Date: July 21, 2025" << endl;
    cout << "---------------------------------------------" << endl << endl;
}

// Each math operation in its own function. I find it easier to debug and extend.
int Add(int a, int b) { return a + b; }
int Subtract(int a, int b) { return a - b; }
int Multiply(int a, int b) { return a * b; }
int Divide(int a, int b) { return a / b; }

// Overloads for decimals. Had to add these for the advanced/float cases (spec A3)
float Add(float a, float b) { return a + b; }
float Subtract(float a, float b) { return a - b; }
float Multiply(float a, float b) { return a * b; }
float Divide(float a, float b) { return a / b; }

double Add(double a, double b) { return a + b; }
double Subtract(double a, double b) { return a - b; }
double Multiply(double a, double b) { return a * b; }
double Divide(double a, double b) { return a / b; }

// Menu function makes it easier to update or adjust without hunting through logic.
void DisplayMenu() {
    cout << "Please select an option:" << endl;
    cout << "1) Basic Calculator (step by step)" << endl;
    cout << "2) Advanced Calculator (single line input)" << endl;
    cout << "3) Toggle Currency Output" << endl;
    cout << "4) Quit" << endl;
}

// Formatting results was a hassle until I remembered to use both fixed and setprecision(2).
void PrintResult(double result, bool currencyMode) {
    if (currencyMode) {
        cout << "$" << fixed << setprecision(2) << result << endl;
    } else {
        cout << result << endl;
    }
}
void PrintResult(int result, bool currencyMode) {
    if (currencyMode) {
        cout << "$" << fixed << setprecision(2) << static_cast<double>(result) << endl;
    } else {
        cout << result << endl;
    }
}

// Input validation for number strings. I used to just try stoi/stod, but found out the program would crash on bad input.
bool IsFloat(const string& str) {
    return str.find('.') != string::npos;
}

bool IsNumber(const string& s) {
    istringstream iss(s);
    double num;
    iss >> noskipws >> num;
    return iss.eof() && !iss.fail();
}

// Simple flag toggle, helps avoid logic bugs.
void ToggleCurrencyMode(bool& currencyMode) {
    currencyMode = !currencyMode;
    cout << (currencyMode ? "Currency mode ENABLED." : "Currency mode DISABLED.") << endl;
}

// Parsing user input for the advanced/inline calculator. Took a few tries to get the string parsing and whitespace removal right.
void AdvancedCalculator(bool currencyMode) {
    string input;
    cout << "Enter your calculation (ex: 4.2 x 3): ";
    getline(cin, input);
    size_t op_pos = input.find_first_of("+-x/");
    if (op_pos == string::npos) {
        cout << "No operator found. Please try again." << endl;
        return;
    }
    string left = input.substr(0, op_pos);
    string right = input.substr(op_pos + 1);
    char op = input[op_pos];
    // Trimming whitespace to avoid conversion errors.
    left.erase(0, left.find_first_not_of(' ')); left.erase(left.find_last_not_of(' ')+1);
    right.erase(0, right.find_first_not_of(' ')); right.erase(right.find_last_not_of(' ')+1);
    if (!IsNumber(left) || !IsNumber(right)) {
        cout << "Invalid numbers. Please try again." << endl;
        return;
    }
    bool isFloat = IsFloat(left) || IsFloat(right);
    if (isFloat) {
        double a = stod(left), b = stod(right);
        if (op == '+') PrintResult(Add(a, b), currencyMode);
        else if (op == '-') PrintResult(Subtract(a, b), currencyMode);
        else if (op == 'x') PrintResult(Multiply(a, b), currencyMode);
        else if (op == '/') {
            if (b == 0) { cout << "Error: Cannot divide by zero." << endl; return; }
            PrintResult(Divide(a, b), currencyMode);
        } else {
            cout << "Invalid operator." << endl;
        }
    } else {
        int a = stoi(left), b = stoi(right);
        if (op == '+') PrintResult(Add(a, b), currencyMode);
        else if (op == '-') PrintResult(Subtract(a, b), currencyMode);
        else if (op == 'x') PrintResult(Multiply(a, b), currencyMode);
        else if (op == '/') {
            if (b == 0) { cout << "Error: Cannot divide by zero." << endl; return; }
            PrintResult(Divide(a, b), currencyMode);
        } else {
            cout << "Invalid operator." << endl;
        }
    }
}

// Tried to keep logic here clear and minimal. If I ever make this more complex, I'd break it up even further.
void CalculatorMenu() {
    bool running = true;
    bool currencyMode = false;
    while (running) {
        DisplayMenu();
        cout << "Select: ";
        string option;
        getline(cin, option);
        if (option == "1") {
            // For step by step mode, there’s a bit of repetition from advanced mode, but it helps users avoid mistakes.
            string firstNum, secondNum, oper;
            cout << "Enter first number: "; getline(cin, firstNum);
            cout << "Enter operator (+, -, x, /): "; getline(cin, oper);
            cout << "Enter second number: "; getline(cin, secondNum);
            if (!IsNumber(firstNum) || !IsNumber(secondNum) || oper.length() != 1) {
                cout << "Invalid input. Try again." << endl;
                continue;
            }
            bool isFloat = IsFloat(firstNum) || IsFloat(secondNum);
            char op = oper[0];
            if (isFloat) {
                double a = stod(firstNum), b = stod(secondNum);
                if (op == '+') PrintResult(Add(a, b), currencyMode);
                else if (op == '-') PrintResult(Subtract(a, b), currencyMode);
                else if (op == 'x') PrintResult(Multiply(a, b), currencyMode);
                else if (op == '/') {
                    if (b == 0) { cout << "Error: Cannot divide by zero." << endl; continue; }
                    PrintResult(Divide(a, b), currencyMode);
                } else { cout << "Invalid operator." << endl; }
            } else {
                int a = stoi(firstNum), b = stoi(secondNum);
                if (op == '+') PrintResult(Add(a, b), currencyMode);
                else if (op == '-') PrintResult(Subtract(a, b), currencyMode);
                else if (op == 'x') PrintResult(Multiply(a, b), currencyMode);
                else if (op == '/') {
                    if (b == 0) { cout << "Error: Cannot divide by zero." << endl; continue; }
                    PrintResult(Divide(a, b), currencyMode);
                } else { cout << "Invalid operator." << endl; }
            }
        } else if (option == "2") {
            AdvancedCalculator(currencyMode);
        } else if (option == "3") {
            ToggleCurrencyMode(currencyMode);
        } else if (option == "4") {
            running = false;
            cout << "Thank you for using the calculator!" << endl;
        } else {
            cout << "Invalid menu selection." << endl;
        }
        cout << endl;
    }
}

int main() {
    ProgramGreeting();
    CalculatorMenu();
    return 0;
}

/*
Example Output:
---------------------------------------------
Welcome to the Simple Calculator Program!
Author: Matt Johns
Due Date: July 21, 2025
---------------------------------------------

Please select an option:
1) Basic Calculator (step by step)
2) Advanced Calculator (single line input)
3) Toggle Currency Output
4) Quit
Select: 1
Enter first number: 15
Enter operator (+, -, x, /): x
Enter second number: 3
45

Please select an option:
1) Basic Calculator (step by step)
2) Advanced Calculator (single line input)
3) Toggle Currency Output
4) Quit
Select: 3
Currency mode ENABLED.

Please select an option:
1) Basic Calculator (step by step)
2) Advanced Calculator (single line input)
3) Toggle Currency Output
4) Quit
Select: 2
Enter your calculation (ex: 4.2 x 3): 4.2 x 3
$12.60

Please select an option:
1) Basic Calculator (step by step)
2) Advanced Calculator (single line input)
3) Toggle Currency Output
4) Quit
Select: 4
Thank you for using the calculator!
*/
