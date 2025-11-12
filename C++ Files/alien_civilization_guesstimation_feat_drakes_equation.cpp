// C3-Source File Heading
/* Drakes Equation.cpp
 Matthew Johns, CISP360
 Homework #2 Due 06/22/2025
 This program calculates the number of detectable civilizations using the Drake Equation,
 applied to the Andromeda Galaxy using custom-labeled variables. */

#include <iostream>
#include <iomanip>

using namespace std;

int main() {

// C1-Declaring Variables
    const int AStrIsBrn = 35;             // rStar: Rate of Star Creation
    const float Suns = 0.45;              // pSwP: % of stars with planets
    const float GoldyLoxZone = 0.7;       // aLife: Avg. habitable planets per system
    const float LetThereBe = 0.80;        // fLife: % where life develops
    const float smrt = 0.30;              // iLife: % of life that becomes intelligent
    const float ETPhoneHome = 1.00;       // comm: % willing to communicate
    const int LifeSpan = 10000;           // civ_life: Expected civilization lifetime (yrs)

// C2-Program Greeting
    cout << "This program calculates the number of potential civilizations\n";
    cout << "in the Andromeda Galaxy using the Drake Equation.\n" << endl;

// B1-Drakes Equation
    float etCiv = static_cast<float>(AStrIsBrn) * Suns * GoldyLoxZone *
                  LetThereBe * smrt * ETPhoneHome * static_cast<float>(LifeSpan);

// C3-Program Output Format
    cout << fixed << setprecision(3);

// A4-Calculation Output Heading
    cout << "\nCHANCE OF INTELLIGENT LIFE\n";
    cout << "===========================\n";
    cout << etCiv << endl;

// A1-Calculation Output Headers
    cout << "\nResults\n";
    cout << "=======\n";
    cout << "Estimated civilizations: " << etCiv << endl;

// B2-Double Output
    cout << "Double results: " << etCiv * 2 << endl;

// B3-Half Output
    cout << "Half results: " << etCiv / 2 << endl;

// A3-Program Output Variable Heading
    cout << "\nVARIABLE NAMES AND VALUES\n";
    cout << "===========================\n";

// A2-Display Variables
    cout << "rStar:     " << AStrIsBrn << endl;
    cout << "pSwP:      " << Suns << endl;
    cout << "aLife:     " << GoldyLoxZone << endl;
    cout << "fLife:     " << LetThereBe << endl;
    cout << "iLife:     " << smrt << endl;
    cout << "comm:      " << ETPhoneHome << endl;
    cout << "civ_life:  " << LifeSpan << endl;

// B4-Drakes Equation Ending
    int pEnd;
    cout << "\nProgram finished. Enter any key to close..." << endl;
    cin >> pEnd;

    return 0;
}


/* A2-Sample Output

CHANCE OF INTELLIGENT LIFE
==========================
8265.000

Results
=======
Double results: 16530.000
Half results: 4132.500

VARIABLE NAMES AND VALUES
===========================
rStar:     35
pSwP:      0.450
aLife:     0.700
fLife:     0.800
iLife:     0.300
comm:      1.000
civ_life:  10000
*/
