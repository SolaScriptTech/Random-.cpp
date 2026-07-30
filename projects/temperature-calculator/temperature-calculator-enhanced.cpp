// CatCotCapTemperature.cpp
// By Matt Johns, CISP 360
// July 11, 2025

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

// This is the opening message for the Program 
void ProgramGreeting() {
    cout << "Welcome to Matt's ridiculously charming Cat, Cot, Cap Temperature Calculator!" << endl;
    cout << "Brought to you by Matt Johns, obviously." << endl;
    time_t t = time(0);
    tm* now = localtime(&t);
    cout << (now->tm_mon + 1) << "/" << now->tm_mday << "/" << (now->tm_year + 1900) << endl;
}

// Spec A2 - Validate word input, true if correct
// This function takes a string (the object name entered by the user),
// converts it to lowercase (so 'CaT' becomes 'cat'), then checks if it matches
// any of our acceptable objects: "cat", "cot", "cap", or "dog". If it does, it returns true.
// Otherwise, we send the user back to the drawing board.
bool ValWord(string inputWord) {
    transform(inputWord.begin(), inputWord.end(), inputWord.begin(), ::tolower); // convert all letters in the word to lowercase
    return (inputWord == "cat" || inputWord == "cot" || inputWord == "cap" || inputWord == "dog"); // check for matches
}

// Spec B2 - Validate temperatures within proper range
// This function ensures that the user's entered temperature is acceptable for the given object.
// If the object is a cat, cot, cap, or dog, it checks whether the temperature falls within its safe range.
// If the temperature is out of bounds, it returns false to trigger a retry.
bool ValFlo(float temp, string obj) {
    if (obj == "cat") return temp >= 101.0 && temp <= 102.2;      // Cats are hot little monsters
    if (obj == "cot") return temp >= 50.0 && temp <= 100.0;       // Cots are warmer than you'd think
    if (obj == "cap") return temp >= 30.0 && temp <= 90.0;        // Caps are chill
    if (obj == "dog") return temp >= 99.0 && temp <= 102.0;       // Doggos keep it toasty
    return false; // If none match, return false (shouldn't happen if obj is validated before)
}

// Spec B3 - Prompt overload (float)
// This is one of two Prompt functions (overloaded). This one is used when we expect a float.
// It takes a string to display as a prompt and returns the number the user typed.
// If they type garbage, we kindly ignore it and ask again (thanks to cin.clear and cin.ignore).
float Prompt(string prompt) {
    cout << prompt;
    float value;
    while (!(cin >> value)) {
        cin.clear(); // clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // skip bad input
        cout << "That doesn't look like a number. Try again: ";
    }
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
// This function prints out a charming little menu that lets the user decide which scale they’d like
// to convert their temperature into. You know, in case they suddenly feel more Celsius than Kelvin.
void DisplayMenu() {
    cout << "Choose the scale to convert the temperature to:\n"; // instructions for the user
    cout << "1. Celsius\n2. Kelvin\n3. Rankine\n4. All of 'em\n"; // the scale options
}

// Spec C1 - Validate object word input
// This function keeps bugging the user until they enter a valid object name (cat, cot, cap, dog).
// It’s immune to weird casing like “CaT” or “DOG”. If they mess up, it gently roasts them.
string GetValidObject() {
    string obj;
    do {
        obj = Prompt(1, "Enter an object (cat, cot, cap, dog): "); // ask for input
        if (!ValWord(obj)) cout << "Woah, tiger! Let's keep this to cats, cots, caps, or dogs.\n"; // friendly jab
    } while (!ValWord(obj)); // loop until valid
    transform(obj.begin(), obj.end(), obj.begin(), ::tolower); // lowercase it just to be sure
    return obj;
}

// Conversion functions
// Each of these functions takes a Fahrenheit temperature and returns the converted value.
// Because sometimes, 98.6 just isn’t enough.

// Fahrenheit to Celsius
float FahrenheitToCelsius(float f) {
    return (f - 32.0) * 5.0 / 9.0; // classic cold math
}

// Fahrenheit to Kelvin
float FahrenheitToKelvin(float f) {
    return ((f - 32.0) * 5.0 / 9.0) + 273.15; // science-y warmness
}

// Fahrenheit to Rankine
float FahrenheitToRankine(float f) {
    return f + 459.67; // total energy extravaganza
}

// Spec A3 - Validate menu option
// This function shows the conversion menu and makes sure the user picks a valid number.
// If they go rogue and pick, say, 17 – they’re kindly nudged back in line.
int GetMenuOption() {
    int option;
    do {
        DisplayMenu(); // show the menu
        option = static_cast<int>(Prompt("Enter your choice: ")); // get user input
        if (option < 1 || option > 4)
            cout << "Alrighty, Deadpool. Pick something from the list.\n"; // try again with style
    } while (option < 1 || option > 4); // repeat if invalid
    return option;
}

int main() {
    ProgramGreeting(); // Display the greeting and system date

    string obj = GetValidObject(); // Get a valid object name from the user

    float temp;
    do {
        temp = Prompt("Enter the Fahrenheit temperature for your " + obj + ": "); // ask for temperature
        if (!ValFlo(temp, obj)) // validate it
            cout << "Nope! That temp won't work for a " << obj << ". Let's try again, shall we?\n";
    } while (!ValFlo(temp, obj)); // repeat if invalid

    int option = GetMenuOption(); // Ask the user which scale they want

    cout << fixed << setprecision(2); // set output to 2 decimal places for classiness

    // Spec C2 - Perform conversion based on user choice
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

    cout << "Alright, that's it. Go enjoy your perfectly temperate day, you beautiful genius.\n"; // sign off like a legend

    return 0; // mic drop
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
