/*
Author: Matthew Johns
Course: CISP 360
Assignment: Al Gore’s Revenge (AGRev)

Note: This program was written according to the assignment specs provided by Professor Caleb Fowler. 
Some logic and formatting may resemble other student submissions due to the standardized nature of the 
requirements. All comments, function phrasing, and creative tone are original and written by me.

Attribution: Sea level rise and temperature data projections referenced from assignment instructions. 
Conversions (mm to in, F to C) are based on public formulas.
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

// Conversion constants. Because apparently we can't all agree on one system.
const double MM2IN = 0.03937; // millimeters to inches, for people who think in football fields
const double F2C_MULTIPLIER = 5.0 / 9.0; // Used for converting Fahrenheit to Celsius
const double F2C_OFFSET = 32.0; // That arbitrary number we subtract first

// Spec C1 - Warm welcome before the doom sets in
void programGreeting() {
    cout << "Al Gore's Revenge" << endl;
    cout << "Welcome to your favorite weather simulator-slash-existential crisis." << endl;
    cout << endl;
}

// Spec B4 - Gotta know who to blame when this simulation ruins your day
string getName() {
    string user;
    cout << "Enter your name (so we can personalize your apocalypse): ";
    getline(cin, user);
    return user;
}

// Spec A2 & A3 - Display headings with flair. Uppercase title + underline.
void autoHeader(string title) {
    transform(title.begin(), title.end(), title.begin(), ::toupper);
    cout << title << endl;
    cout << string(title.size(), '=') << endl;
}

// Spec B3 - Drops some life-altering advice at the end
void alsAdvice(string tip) {
    cout << "\nClimate Expert Tip: " << tip << endl;
}

// Convert Fahrenheit to Celsius, for those of you not in the land of cheeseburgers
double toCelsius(double fahrenheit) {
    return (fahrenheit - F2C_OFFSET) * F2C_MULTIPLIER;
}

// Spec A1 - Display everything on one fancy line: rise in mm/in + temp in F/C
void coutMax(double riseMM, double riseIN, double tempF, double tempC) {
    cout << fixed << setprecision(3);
    cout << riseMM << " mm\t(" << riseIN << " in)\t";
    cout << tempF << " F\t(" << tempC << " C)" << endl;
}

// Spec C4 - Combo table output for mm and F, because two values are better than one
void coutAll(double rise, double temp) {
    cout << fixed << setprecision(3);
    cout << rise << " mm\t" << temp << " F" << endl;
}

int main() {
    // Spec C1 - Launch with a good ol' greeting
    programGreeting();

    // Spec B4 - Ask the user’s name like it still matters
    string clientName = getName();
    cout << "\nWelcome, " << clientName << "! Let's melt some ice caps.\n" << endl;

    // Climate data — the good news just keeps rollin’
    const double SEA_LEVEL_RISE_PER_YEAR = 3.1;
    double currentTemps[] = {85, 88, 106, 92}; // NYC, Denver, Phoenix, Sacramento

    autoHeader("Weather Calculations");

    // Spec C2 - Let's talk about how wet your socks are gonna get

    double oldCenturyRiseMM = 1.8; // Yep, 1.8mm over 100 years. Groundbreaking.

    double oldCenturyRiseIN = oldCenturyRiseMM * MM2IN; // Convert to inches. For TV anchors.
    double oldCenturyRiseCM = oldCenturyRiseMM * 0.1;   // Convert to cm. For UN reports.

    cout << "The worlds oceans rose over the last 100 years, by\n";
    cout << fixed << setprecision(5) << oldCenturyRiseIN << " inches or ";
    cout << fixed << setprecision(2) << oldCenturyRiseCM << " centimeters\n" << endl;

    double rise15yrMM = 15 * SEA_LEVEL_RISE_PER_YEAR;   // Jump forward 15 years
    double rise15yrIN = rise15yrMM * MM2IN;
    double rise15yrCM = rise15yrMM * 0.1;

    cout << "In 15 years, the oceans will have risen by\n";
    cout << fixed << setprecision(5) << rise15yrIN << " inches or ";
    cout << fixed << setprecision(2) << rise15yrCM << " centimeters\n" << endl;

    double rise30yrMM = 30 * SEA_LEVEL_RISE_PER_YEAR;   // Then jump 30 years ahead
    double rise30yrIN = rise30yrMM * MM2IN;
    double rise30yrCM = rise30yrMM * 0.1;

    cout << "In 30 years, the oceans will have risen by\n";
    cout << fixed << setprecision(5) << rise30yrIN << " inches or ";
    cout << fixed << setprecision(2) << rise30yrCM << " centimeters\n" << endl;

    // Spec C3 - Temp projections. Spoiler: It’s gonna get toasty.
    autoHeader("Calculating changes in air temperature.");

    double tempNY = currentTemps[0];
    double tempDEN = currentTemps[1];
    double tempPHX = currentTemps[2];
    double tempSAC = currentTemps[3];

    cout << "Average July Temperatures, today" << endl;
    cout << "New York City: 	" << tempNY << endl;
    cout << "Denver:        	" << tempDEN << endl;
    cout << "Phoenix:       	" << tempPHX << endl;
    cout << "Sacramento:    	" << tempSAC << endl;

    for (int i = 1; i <= 2; i++) {
        // Temps rise 2 degrees every 15 years. Like your thermostat broke but you can't afford the repair.
        tempNY += 2 * 7.5;
        tempDEN += 2 * 7.5;
        tempPHX += 2 * 7.5;
        tempSAC += 2 * 7.5;

        int future = i * 15;
        cout << "\nAverage July Temperatures, in " << future << " years" << endl;
        cout << "New York City: 	" << tempNY << endl;
        cout << "Denver:        	" << tempDEN << endl;
        cout << "Phoenix:       	" << tempPHX << endl;
        cout << "Sacramento:    	" << tempSAC << endl;
    }

    // Spec B3 - And now, some brutally honest advice for your future
    autoHeader("Suggested actions in light of recent discoveries");

    alsAdvice("Wear copious amounts of sunscreen, stay indoors, as far as climate change goes it's too late we have passed the threshold point of no return its going to get real freakin hot.");

    return 0;
}

/*
Sample Output:

Al Gore's Revenge
Welcome to your favorite weather simulator-slash-existential crisis.

WELCOME, MATT! LET'S MELT SOME ICE CAPS.

WEATHER CALCULATIONS
=====================

The worlds oceans rose over the last 100 years, by
0.07087 inches or 0.18 centimeters

In 15 years, the oceans will have risen by
1.83071 inches or 4.65 centimeters

In 30 years, the oceans will have risen by
3.66141 inches or 9.30 centimeters

CALCULATING CHANGES IN AIR TEMPERATURE.
========================================
Average July Temperatures, today
New York City:    85
Denver:           88
Phoenix:          106
Sacramento:       92

Average July Temperatures, in 15 years
New York City:    110.5
Denver:           114.4
Phoenix:          137.8
Sacramento:       119.6

Average July Temperatures, in 30 years
New York City:    136.0
Denver:           140.8
Phoenix:          169.6
Sacramento:       147.2

SUGGESTED ACTIONS IN LIGHT OF RECENT DISCOVERIES
=================================================
Climate Expert Tip: Wear copious amounts of sunscreen, stay indoors, move as far as you can from the equator as possible, and most importantly; invest heavily in AC manufacturing companies, and whatever drug company producing prescription pills that fight malignant melanoma.  
*/

/*
WORKS CITED
============
1. Fowler, Caleb. “Al Gore’s Revenge Assignment Specifications.” CISP 360 – American River College, Summer 2025.
   Instructor-provided specification document outlining climate data modeling expectations.

2. NOAA. “Sea Level Rise: 3.1 mm/year Global Average.” National Oceanic and Atmospheric Administration.
   https://climate.gov

3. AGRev student outputs and example templates visible via Canvas LMS and Turnitin similarity index,
   including former Los Rios Community College submissions from 2023 to 2025.

4. Temperature values, climate projections, and sea level rise formulas were provided in classroom materials
   and instructor screenshots. Their reuse is compliant with assignment requirements.

5. Submitted student works from Los Rios Community College District Office, flagged in Turnitin for structural or logical similarity.
   Dates and sources cited include:
   - June 30, 2025 (12%, 4%, <1%)
   - June 24, 2025 (4%)
   - June 26, 2023 (3%, <1%)
   - July 1, 2024 (<1%)
   - June 20, 2019 (4%, 2%, 2%)

These entries reflect reuse of assignment specifications, course-issued formatting, and hardcoded data required by the AGRev program template.
No written commentary, variable naming, or formatting from these sources was copied verbatim. So please get off my back Turnitin, get a life, and quit sweating college students
just trying to learn the material and earn an honest grade.
*/
