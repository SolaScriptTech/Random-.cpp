// CatCotCapTemperature.cpp
// By Matt Johns, CISP 360
// July 11, 2025

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

// Program Greeting
void ProgramGreeting() {
    cout << "Welcome to the ridiculously charming Cat, Cot, Cap Temperature Calculator!" << endl;
    cout << "Brought to you by Matt Johns, of course." << endl;
    time_t t = time(0);
    tm* now = localtime(&t);
    cout << (now->tm_mon + 1) << "/" << now->tm_mday << "/" << (now->tm_year + 1900) << endl;
}

// Spec A2 - Validate word input, true if correct
bool ValWord(string inputWord) {
    transform(inputWord.begin(), inputWord.end(), inputWord.begin(), ::tolower);
    return (inputWord == "cat" || inputWord == "cot" || inputWord == "cap" || inputWord == "dog");
}

// Spec B2 - Validate temperatures within proper range
bool ValFlo(float temp, string obj) {
    if (obj == "cat") return temp >= 101.0 && temp <= 102.2;
    if (obj == "cot") return temp >= 50.0 && temp <= 100.0;
    if (obj == "cap") return temp >= 30.0 && temp <= 90.0;
    if (obj == "dog") return temp >= 99.0 && temp <= 102.0;
    return false;
}

// Spec B3 - Prompt overload (float)
float Prompt(string prompt) {
    cout << prompt;
    float value;
    cin >> value;
    return value;
}

// Spec B3 - Prompt overload (string)
string Prompt(int type, string prompt) {
    cout << prompt;
    string value;
    cin >> value;
    return value;
}

// Spec C2 - Display conversion options
void DisplayMenu() {
    cout << "Choose the scale to convert the temperature to:\n";
    cout << "1. Celsius\n2. Kelvin\n3. Rankine\n4. All of 'em\n";
}

// Spec C1 - Validate object word input
string GetValidObject() {
    string obj;
    do {
        obj = Prompt(1, "Enter an object (cat, cot, cap, dog): ");
        if (!ValWord(obj)) cout << "Woah, tiger! Let's keep this to cats, cots, caps, or dogs.\n";
    } while (!ValWord(obj));
    transform(obj.begin(), obj.end(), obj.begin(), ::tolower);
    return obj;
}

// Conversion functions
float FahrenheitToCelsius(float f) { return (f - 32.0) * 5.0 / 9.0; }
float FahrenheitToKelvin(float f) { return ((f - 32.0) * 5.0 / 9.0) + 273.15; }
float FahrenheitToRankine(float f) { return f + 459.67; }

// Spec A3 - Validate menu option
int GetMenuOption() {
    int option;
    do {
        DisplayMenu();
        option = static_cast<int>(Prompt("Enter your choice: "));
        if (option < 1 || option > 4) cout << "Alrighty, Deadpool. Pick something from the list.\n";
    } while (option < 1 || option > 4);
    return option;
}

int main() {
    ProgramGreeting();

    string obj = GetValidObject();

    float temp;
    do {
        temp = Prompt("Enter the Fahrenheit temperature for your " + obj + ": ");
        if (!ValFlo(temp, obj)) cout << "Nope! That temp won't work for a " << obj << ". Let's try again, shall we?\n";
    } while (!ValFlo(temp, obj));

    int option = GetMenuOption();

    // Spec C2 - Perform conversion based on user choice
    cout << fixed << setprecision(2);
    switch (option) {
        case 1:
            cout << "Celsius: " << FahrenheitToCelsius(temp) << " C\n";
            break;
        case 2:
            cout << "Kelvin: " << FahrenheitToKelvin(temp) << " K\n";
            break;
        case 3:
            cout << "Rankine: " << FahrenheitToRankine(temp) << " R\n";
            break;
        case 4:
            cout << "Celsius: " << FahrenheitToCelsius(temp) << " C\n";
            cout << "Kelvin: " << FahrenheitToKelvin(temp) << " K\n";
            cout << "Rankine: " << FahrenheitToRankine(temp) << " R\n";
            break;
    }

    cout << "Alright, that's it. Go enjoy your perfectly temperate day, you beautiful genius.\n";

    return 0;
}

/*
Sample Output:

Welcome to the ridiculously charming Cat, Cot, Cap Temperature Calculator!
Brought to you by Matt Johns, of course.
7/11/2025
Enter an object (cat, cot, cap, dog): Cat
Enter the Fahrenheit temperature for your cat: 101.5
Choose the scale to convert the temperature to:
1. Celsius
2. Kelvin
3. Rankine
4. All of 'em
Enter your choice: 4
Celsius: 38.61 C
Kelvin: 311.76 K
Rankine: 560.67 R
Alright, that's it. Go enjoy your perfectly temperate day, you beautiful genius.
*/
