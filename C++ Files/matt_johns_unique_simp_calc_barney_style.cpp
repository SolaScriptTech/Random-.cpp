// Matt_Johns_Unique_SimpCalc_BarneyStyle.cpp
// Matt Johns, CISP 360
// July 16, 2025
// Simple Calculator Project (All Specs Covered)
//
// This is a simple calculator program that follows all the assignment specs. Every part of the code is commented in simple language. If you've never coded before, don't worry: every piece is explained just like Barney would explain it to a friend!
//
// Assignment Specs Covered:
//  C1: Each operation (+, -, x, /) gets its own function.
//  C2: There is a menu made of two functions: one to show choices, one to act on your choice.
//  C3: Menu logic that picks the correct math job.
//  B1: There's a mode to make all numbers look like money, if you want.
//  B2: No dividing by zero allowed.
//  B3: Calculator keeps going until you say stop.
//  A1: You can type the whole math problem in one line (like 2 + 2).
//  A2: Calculator can work with both whole numbers and numbers with a dot (like 7.5).
//  A3: Operation functions work for both number types, using C++ overloads.

#include <iostream>
#include <iomanip>  // lets us use setprecision for money
#include <string>   // lets us use strings (words)
#include <sstream>  // lets us change strings to numbers
using namespace std;

// Spec C1: Here we make one function for each kind of math job (add, subtract, multiply, divide)
int Add(int a, int b) { return a + b; }
int Subtract(int a, int b) { return a - b; }
int Multiply(int a, int b) { return a * b; }
int Divide(int a, int b) { return a / b; } // Caller must check b is not 0 first!

// Spec A3: Do the same but for decimal numbers (floats/doubles)
double Add(double a, double b) { return a + b; }
double Subtract(double a, double b) { return a - b; }
double Multiply(double a, double b) { return a * b; }
double Divide(double a, double b) { return a / b; }

// Function that shows a welcome message with assignment details (Spec: Greeting)
void ProgramGreeting() {
    cout << "==============================\n";
    cout << "Welcome to Barney's Calculator!\n";
    cout << "By: Matt Johns\n";
    cout << "Assignment Due: July 21, 2025\n";
    cout << "==============================\n" << endl;
}

// Spec C2: Show the menu of what you can do
void ShowMenu() {
    cout << "Menu - What do you want to do?" << endl;
    cout << "1) Step by Step Calculator" << endl;
    cout << "2) Type Whole Problem (like 8.5 + 7)" << endl;
    cout << "3) Turn On/Off Money Format" << endl;
    cout << "4) Quit Calculator" << endl;
}

// Function to check if a string looks like a number (whole or decimal)
bool IsNumber(const string& s) {
    istringstream iss(s);
    double test;
    iss >> noskipws >> test;
    return iss.eof() && !iss.fail();
}

// Function to check if number has a dot (like 4.3)
bool IsFloat(const string& s) {
    return s.find('.') != string::npos;
}

// Prints a number the normal way or as money ($x.xx)
void PrintNumber(double value, bool moneyMode) {
    if (moneyMode) cout << "$" << fixed << setprecision(2) << value;
    else cout << value;
}
void PrintNumber(int value, bool moneyMode) {
    if (moneyMode) cout << "$" << fixed << setprecision(2) << (double)value;
    else cout << value;
}

// Spec B1: Turns money format on/off
void ToggleMoneyMode(bool& mode) {
    mode = !mode; // Flip it to opposite of what it was
    cout << (mode ? "Money mode is ON. Every answer will look like money!" : "Money mode is OFF. Numbers look normal.") << endl;
}

// Spec B3: The big loop! Keeps calculator running until user says quit
void MainLoop() {
    bool keepGoing = true;        // If true, we keep going
    bool moneyMode = false;       // If true, answers show as money
    string userPick;              // What user picks from menu

    while (keepGoing) {
        ShowMenu();               // Show choices
        cout << "Pick a number (1-4): ";
        getline(cin, userPick);   // User types choice
        if (userPick == "1") {
            // --- Step-by-step calculator (asks one thing at a time) ---
            string first, op, second;
            cout << "Type first number: "; getline(cin, first);
            cout << "Pick operator (+, -, x, /): "; getline(cin, op);
            cout << "Type second number: "; getline(cin, second);
            // Check if numbers are real
            if (!IsNumber(first) || !IsNumber(second) || op.length() != 1) {
                cout << "Oops! Please type real numbers and a correct operator." << endl;
                continue;
            }
            // Spec A2: If it has a dot, use double math, else int math
            bool isFloat = IsFloat(first) || IsFloat(second);
            char whichOp = op[0];
            if (isFloat) {
                double a = stod(first); double b = stod(second); double result;
                if (whichOp == '+') result = Add(a, b);
                else if (whichOp == '-') result = Subtract(a, b);
                else if (whichOp == 'x') result = Multiply(a, b);
                else if (whichOp == '/') {
                    if (b == 0) { cout << "Uh oh! Can't divide by zero!" << endl; continue; }
                    result = Divide(a, b);
                } else { cout << "That's not a real operator." << endl; continue; }
                cout << a << " " << whichOp << " " << b << " = "; PrintNumber(result, moneyMode); cout << endl;
            } else {
                int a = stoi(first); int b = stoi(second); int result;
                if (whichOp == '+') result = Add(a, b);
                else if (whichOp == '-') result = Subtract(a, b);
                else if (whichOp == 'x') result = Multiply(a, b);
                else if (whichOp == '/') {
                    if (b == 0) { cout << "Uh oh! Can't divide by zero!" << endl; continue; }
                    result = Divide(a, b);
                } else { cout << "That's not a real operator." << endl; continue; }
                cout << a << " " << whichOp << " " << b << " = "; PrintNumber(result, moneyMode); cout << endl;
            }
        } else if (userPick == "2") {
            // --- Spec A1: Let user type the whole math problem ---
            string problem;
            cout << "Type your math (example: 5.5 x 4): ";
            getline(cin, problem);
            size_t whereOp = problem.find_first_of("+-x/");
            if (whereOp == string::npos) { cout << "I couldn't find an operator. Try again!" << endl; continue; }
            string left = problem.substr(0, whereOp);
            string right = problem.substr(whereOp+1);
            char op = problem[whereOp];
            // Remove spaces from start and end
            left.erase(0, left.find_first_not_of(' ')); left.erase(left.find_last_not_of(' ')+1);
            right.erase(0, right.find_first_not_of(' ')); right.erase(right.find_last_not_of(' ')+1);
            // Check if both are numbers
            if (!IsNumber(left) || !IsNumber(right)) { cout << "Those aren't both numbers!" << endl; continue; }
            bool isFloat = IsFloat(left) || IsFloat(right);
            if (isFloat) {
                double a = stod(left); double b = stod(right); double result;
                if (op == '+') result = Add(a, b);
                else if (op == '-') result = Subtract(a, b);
                else if (op == 'x') result = Multiply(a, b);
                else if (op == '/') {
                    if (b == 0) { cout << "Can't divide by zero! Try again." << endl; continue; }
                    result = Divide(a, b);
                } else { cout << "Not a real operator!" << endl; continue; }
                cout << a << " " << op << " " << b << " = "; PrintNumber(result, moneyMode); cout << endl;
            } else {
                int a = stoi(left); int b = stoi(right); int result;
                if (op == '+') result = Add(a, b);
                else if (op == '-') result = Subtract(a, b);
                else if (op == 'x') result = Multiply(a, b);
                else if (op == '/') {
                    if (b == 0) { cout << "Can't divide by zero! Try again." << endl; continue; }
                    result = Divide(a, b);
                } else { cout << "Not a real operator!" << endl; continue; }
                cout << a << " " << op << " " << b << " = "; PrintNumber(result, moneyMode); cout << endl;
            }
        } else if (userPick == "3") {
            // --- Spec B1: Turn money mode on/off ---
            ToggleMoneyMode(moneyMode);
        } else if (userPick == "4") {
            // --- Quit! ---
            keepGoing = false;
            cout << "Thanks for using Barney's Calculator! Bye!" << endl;
        } else {
            cout << "I didn't get that. Pick 1, 2, 3 or 4." << endl;
        }
        cout << endl; // Blank line to keep things neat!
    }
}

// The magic begins here! (main function)
int main() {
    ProgramGreeting(); // Say hi and show what this is
    MainLoop();        // Start the calculator menu loop
    return 0;          // Tell computer we are done (all done!)
}

/*
Projected Output Example (Barney Style):
==============================
Welcome to Barney's Calculator!
By: Matt Johns
Assignment Due: July 21, 2025
==============================

Menu - What do you want to do?
1) Step by Step Calculator
2) Type Whole Problem (like 8.5 + 7)
3) Turn On/Off Money Format
4) Quit Calculator
Pick a number (1-4): 1
Type first number: 8.5
Pick operator (+, -, x, /): x
Type second number: 4
8.5 x 4 = 34

Menu - What do you want to do?
1) Step by Step Calculator
2) Type Whole Problem (like 8.5 + 7)
3) Turn On/Off Money Format
4) Quit Calculator
Pick a number (1-4): 3
Money mode is ON. Every answer will look like money!

Menu - What do you want to do?
1) Step by Step Calculator
2) Type Whole Problem (like 8.5 + 7)
3) Turn On/Off Money Format
4) Quit Calculator
Pick a number (1-4): 2
Type your math (example: 5.5 x 4): 5.5 x 4
5.5 x 4 = $22.00

Menu - What do you want to do?
1) Step by Step Calculator
2) Type Whole Problem (like 8.5 + 7)
3) Turn On/Off Money Format
4) Quit Calculator
Pick a number (1-4): 4
Thanks for using Barney's Calculator! Bye!
*/
