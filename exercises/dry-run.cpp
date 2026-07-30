/*
dryRun.cpp
Matt Johns, CISP 360
June 15, 2025
*/

// Specification C1 – ProgramGreeting
// This program displays "Hello World - my name is Matt Johns" and prints several variables.

#include <iostream>
#include <string>
using namespace std;

int main() {
    // Input Paragraph
    // (No input in this program, but included for structure)

    // Processing Paragraph
    // (No processing logic in this program, just declarations and output)

    // Output Paragraph
    cout << "Program: Dry Run (dry)" << endl;
    cout << "Author: Matt Johns" << endl;
    cout << "Date: June 15, 2025" << endl;
    cout << "Description: This program displays 'Hello World - my name is Matt Johns' and prints several variables." << endl;
    cout << endl;

    cout << "Hello World - my name is Matt Johns" << endl;

    // Specification B1 – Variables
    int myInt = 10;              
    float myFloat = 3.14f;       
    double myDouble = 2.71828;   
    bool myBool = true;          
    char myChar = 'M';           
    string myString = "Matt";    

    /* My Variables are listed in the following order from top to bottom: Integer, Float, Double, Boolean, Character, and String.
       The integer is used to store whole numbers. The float and double are both used for decimal numbers, with double offering more precision.
       The boolean holds true or false values, the character stores a single letter, and the string holds a series of characters like a name.
       These were included to demonstrate my understanding of basic variable declaration and initialization of often used C++ data types. */

    // Specification A1 – Output Variables
    cout << "Integer (myInt): " << myInt << endl;
    cout << "Float (myFloat): " << myFloat << endl;
    cout << "Double (myDouble): " << myDouble << endl;
    cout << "Boolean (myBool): " << myBool << endl;
    cout << "Character (myChar): " << myChar << endl;
    cout << "String (myString): " << myString << endl;

    return 0;
}

/*
==========
Sample Output:
Program: Dry Run (dry)
Author: Matt Johns
Date: June 15, 2025
Description: This program displays 'Hello World - my name is Matt Johns' and prints several variables.

Hello World - my name is Matt Johns
Integer (myInt): 10
Float (myFloat): 3.14
Double (myDouble): 2.71828
Boolean (myBool): 1
Character (myChar): M
String (myString): Matt
==========
*/