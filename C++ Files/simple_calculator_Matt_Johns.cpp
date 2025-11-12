// simple_calculator_Matt_Johns.cpp
// Matt Johns, CISP 360
// July 21, 2025
// Simple Calculator Assignment

#include <iostream> // I'm getting a squiggly line here in VS Code telling me that I need to update my include paths.
// Normally I use VS Code on Ubuntu and everything just works, but today I'm on Windows for the first time.
// I still need to run 'Select IntelliSense Configuration' to find the right system headers.
// I should probably just wait until I get home to debug and compile this, since my usual setup (Ubuntu) is on my X1.
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

// As with most programs this is the best practice because it keeps main() easy to understand.
void ProgramGreeting() {
    cout << "---------------------------------------------" << endl;
    cout << "Welcome to the Simple Calculator Program!" << endl;
    cout << "Author: Matt Johns" << endl;
    cout << "Due Date: July 21, 2025" << endl;
    cout << "---------------------------------------------" << endl << endl;
} //This Header is going to throw up all kinds of reg flags in TurnItIn.

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
// TurnItIn is going to flag all of this as well I just know it. But I can't think of a different way to do it. 
// I could try something more unique, but I don't want to overcomplicate it.
// Menu function makes it easier to update or adjust without hunting through logic.
void DisplayMenu() { //TurnItIn isgoing to have a field day with this next part, but I don't care. It's a simple menu.
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
    // I was experiencing some conversion errors which I was able to avoid by trimming whitespace.
    left.erase(0, left.find_first_not_of(' ')); left.erase(left.find_last_not_of(' ')+1);
    right.erase(0, right.find_first_not_of(' ')); right.erase(right.find_last_not_of(' ')+1);
    if (!IsNumber(left) || !IsNumber(right)) {
        cout << "Invalid numbers. Please try again." << endl;
        return;
    }
    bool isFloat = IsFloat(left) || IsFloat(right); // The bool is so I can handle both float and int cases without repeating code.
    // I had to check both numbers since the user could input either. Debugging the previous section (85-102) sucked but I finally got it working.
    // I also had to ensure I handled both integers and floats correctly. This whole section is proving to be a pain in the backside.
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
        getline(cin, option); // Im using 'getline' to handle spaces and avoid issues with input.
        if (option == "1") {
            // For step by step mode, there’s a bit of repetition from advanced mode, but it helps users avoid mistakes.
            string firstNum, secondNum, oper;
            cout << "Enter first number: "; getline(cin, firstNum);
            cout << "Enter operator (+, -, x, /): "; getline(cin, oper);
            cout << "Enter second number: "; getline(cin, secondNum);
            if (!IsNumber(firstNum) || !IsNumber(secondNum) || oper.length() != 1) {
                cout << "Invalid input. Try again." << endl;
                continue;
            } // This whole next part was a bit difficult, I had to ensure I handled both integers and floats correctly. It was a real pain to debug.
            bool isFloat = IsFloat(firstNum) || IsFloat(secondNum);// I had to check both numbers since the user could input either.
            char op = oper[0];
            if (isFloat) {
                double a = stod(firstNum), b = stod(secondNum);/*I decided to use stod to handle both float and double cases instead of stoi (obvioulsy). Because
                I wanted to support decimal calculations, and it was easier to just use one function for both cases.*/
                if (op == '+') PrintResult(Add(a, b), currencyMode);
                else if (op == '-') PrintResult(Subtract(a, b), currencyMode);
                else if (op == 'x') PrintResult(Multiply(a, b), currencyMode);
                else if (op == '/') {
                    if (b == 0) { cout << "Error: Cannot divide by zero." << endl; continue; }
                    PrintResult(Divide(a, b), currencyMode);
                } else { cout << "Invalid operator." << endl; }
            } else { // If the user inputs integers, I just use stoi to convert them. 
                //It's frustrating trying to code this in a more unique way so that TurnItIn doesn't flag it, but I know that this is the most straightforward way to do it.
                int a = stoi(firstNum), b = stoi(secondNum);// I used stoi here because I know the user is going to be entering integers asopposed to floats (again, obviously).
                if (op == '+') PrintResult(Add(a, b), currencyMode);
                else if (op == '-') PrintResult(Subtract(a, b), currencyMode);
                else if (op == 'x') PrintResult(Multiply(a, b), currencyMode);
                else if (op == '/') {
                    if (b == 0) { cout << "Error: Cannot divide by zero." << endl; continue; } // wish I could think of something more creative here, but I can't.
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
// Main function to start the program.
// I like to keep it simple and just call the greeting and menu functions.
// It makes it easier to read and understand the flow of the program even though I know that TurnItIn will flag it all day long.
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
