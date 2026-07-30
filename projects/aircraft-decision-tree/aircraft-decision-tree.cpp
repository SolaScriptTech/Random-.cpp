// acdt.cpp
// Matt Johns, CISP 360
// 07/08/2025

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
using namespace std;

// Function Prototypes
/*
This function greets the user with some friendly panic prevention.
It's the opening line to the adventure where buttons don't work and radios go silent.
*/
void ProgramGreeting();
/*
This function throws the poor pilot into decision-making mode.
A short list of your worst airborne nightmares. Pick your poison.
*/
void DisplayMenu();
/*
Asks a question and gets an answer. Hopefully.
If you can't type a number here, maybe skip flying today.
*/
int AskThePilot(const string& prompt); // Spec A1: Prompt Function
/*
Checks if the pilot aimed too low with the menu selection.
Like picking zero on a list that starts at 1. Classic.
*/
bool MenuTooLow(int value); // Spec A2: Validate Menu, too low
/*
Checks if the pilot went overboard picking a menu option.
There are only 4 things to pick from, slow down Maverick.
*/
bool MenuTooHigh(int value); // Spec A3: Validate Menu, too high
/*
Verifies your wings aren't trying to cosplay as ice sculptures.
Anything over 10mm? Game over. Less than or equal to 0? What are we even doing?
*/
float ValidateIcingLevel(float userInput); // Spec B3: Validate Icing

int main() {
    ProgramGreeting();

    // Spec C4: Valid N Number
    string tailNumber;
    cout << "Enter your aircraft registration number (N-number): ";
    getline(cin, tailNumber);
    if (tailNumber[0] != 'N' && tailNumber[0] != 'n') {
        cout << "Invalid Registration" << endl;
        return 0;
    }

    // Show menu until user bails out
    int userSelection;
    time_t now = time(0); // Because pilots love knowing what time it is during a crisis
    do {
        DisplayMenu();
        /*
Spec A4: Date
Because nothing calms a pilot like seeing the exact time everything started going wrong.
*/
        cout << "System Time: " << ctime(&now);
        userSelection = AskThePilot("Enter your choice: ");

        // Spec A2/A3: Menu Input Validation
        if (MenuTooLow(userSelection) || MenuTooHigh(userSelection)) {
            cout << "Incorrect input entered! This is why we can't have nice things..." << endl;
            return 0;
        }

        // Spec B4: Switch Menu Option Processing
        switch (userSelection) {
            case 1:
                // Spec C1: Communications Option
                cout << "Oh great, your radio just turned into a paperweight." << endl;
                cout << "Switch to Alternate Radio. And maybe stop spilling coffee on the dashboard." << endl;
                break;
            case 2:
                // Spec C2: Engine Failure Option
                cout << "Engine Failure Protocol Engaged. Deep breaths, champ." << endl;
                if (AskThePilot("Is there engine roughness? (0 = No, 1 = Yes): ")) {
                    if (AskThePilot("Is carb heat on? (0 = No, 1 = Yes): ")) {
                        cout << "Open throttle and lean mixture. Translation: More go-go juice, less air-choke." << endl;
                    } else {
                        cout << "Turn carb heat on. It's not just a fancy button, you know." << endl;
                    }
                } else {
                    if (AskThePilot("Is fuel quantity OK? (0 = No, 1 = Yes): ")) {
                        if (AskThePilot("Is mixture rich? (0 = No, 1 = Yes): ")) {
                            cout << "Check magnetos. Or just scream into the void, your call." << endl;
                        } else {
                            cout << "Enrich mixture. Give the engine what it wants before it quits on you completely." << endl;
                        }
                    } else {
                        cout << "Switch fuel tanks. Maybe next time don't rely on a gas gauge that came out of a cereal box." << endl;
                    }
                }
                break;
            case 3:
                // Spec C3: In-Flight Icing Option
                float icingInput;
                cout << "Enter estimated ice accumulation (0.0 - 10.0 mm): ";
                cin >> icingInput;
                /* Spec B3: Validate Icing */
                float validatedIce = ValidateIcingLevel(icingInput);
                if (validatedIce == 0.0) {
                    cout << "Invalid icing value. Ice must be greater than 0 and less than or equal to 10." << endl;
                    return 0;
                }
                // Spec B2: Icing Table
                cout << fixed << setprecision(1);
                cout << "Confirmed ice: " << validatedIce << " mm." << endl;
                if (validatedIce < 1.0) {
                    cout << "Set deicing power to 5%. Just a polite tap on the wings." << endl;
                } else if (validatedIce <= 5.0) {
                    cout << "Set deicing power to 20%. Nothing dramatic, just a little ice massage." << endl;
                } else if (validatedIce <= 9.0) {
                    cout << "Set deicing power to 65%. Things are getting dicey, huh?" << endl;
                } else {
                    cout << "Set deicing power to 100%. You've officially got a Popsicle for a plane." << endl;
                }
                break;
            case 4:
                cout << "Quitting the program. And hopefully not the flight. Good luck up there, Maverick." << endl << endl;
                break;
        }

    } while (userSelection != 4);

    return 0;
}

// Program Greeting
void ProgramGreeting() {
    cout << "Welcome to the ACDT (Aircraft Catastrophic Decision Tree) Simulator!" << endl;
    cout << "Helping you survive flight school one menu option at a time." << endl << endl;
}

// Menu display
void DisplayMenu() {
    cout << "\n===== Main Menu =====" << endl;
    cout << "1. Communications Failure" << endl;
    cout << "2. Engine Failure" << endl;
    cout << "3. In-Flight Icing" << endl;
    cout << "4. Quit" << endl;
}

/*
Spec A1: Prompt Function
Asks a question and gets an answer. Hopefully.
If you can't type a number here, maybe skip flying today.
*/
int AskThePilot(const string& prompt) {
    int input;
    cout << prompt;
    cin >> input;
    return input;
}

/*
Spec A2: Validate Menu, too low
Checks if the pilot aimed too low with the menu selection.
Like picking zero on a list that starts at 1. Classic.
*/
bool MenuTooLow(int value) {
    return value < 1;
}

/*
Spec A3: Validate Menu, too high
Checks if the pilot went overboard picking a menu option.
There are only 4 things to pick from, slow down Maverick.
*/
bool MenuTooHigh(int value) {
    return value > 4;
}

/*
Spec B3: Validate Icing
Verifies your wings aren't trying to cosplay as ice sculptures.
Anything over 10mm? Game over. Less than or equal to 0? What are we even doing?
*/
float ValidateIcingLevel(float userInput) {
    if (userInput <= 0.0 || userInput > 10.0) {
        return 0.0;
    }
    return userInput;
}

/*
========== SAMPLE OUTPUT ========== 
Welcome to the ACDT (Aircraft Catastrophic Decision Tree) Simulator!
Helping you survive flight school one menu option at a time.

Enter your aircraft registration number (N-number): N123AB

===== Main Menu =====
1. Communications Failure
2. Engine Failure
3. In-Flight Icing
4. Quit
System Time: Tue Jul  8 07:15:00 2025
Enter your choice: 2
Engine Failure Protocol Engaged. Deep breaths, champ.
Is there engine roughness? (0 = No, 1 = Yes): 1
Is carb heat on? (0 = No, 1 = Yes): 0
Turn carb heat on. It's not just a fancy button, you know.

===== Main Menu =====
...
*/