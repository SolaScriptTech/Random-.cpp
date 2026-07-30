// BaseballScore.cpp
// CISP 360 Exam 4, Question 2
// Author: Matt Johns
// Program will compute the final score of a baseball game, including extra innings for tie games.
// User is prompted to enter runs for Team A and Team B for each inning.
// If the game is tied after 9 innings, no more than 3 extra innings can be played.
// The final scores and the winner are declared.

#include <iostream>
using namespace std;

int main() {
    // Initialize scores 
    int teamA = 0, teamB = 0; 
    // Constants for regular and extra innings
    const int REGULAR_INNINGS = 9;
    const int MAX_EXTRA_INNINGS = 3;
    int runsA, runsB;  // Variables for storing the score for each team

    cout << "Team A vs Team B Score Counter" << endl;
    cout << "--------------------------------" << endl;

    // Input runs for 9 regular innings
    for (int inning = 1; inning <= REGULAR_INNINGS; inning++) {
        cout << "Inning " << inning << " - Team A runs: ";
        cin >> runsA; // Reads runs for team A 
        cout << "Inning " << inning << " - Team B runs: ";
        cin >> runsB; // Reads runs for team B
        // Adding total runs
        teamA += runsA;
        teamB += runsB;
    }

    // This tracks the number of innings played
    int inningCount = REGULAR_INNINGS;

    // If tied go to extra innings
    while (teamA == teamB && inningCount < REGULAR_INNINGS + MAX_EXTRA_INNINGS) {
        inningCount++;
        cout << "Extra Inning " << (inningCount - REGULAR_INNINGS) << " - Team A runs: ";
        cin >> runsA;
        cout << "Extra Inning " << (inningCount - REGULAR_INNINGS) << " - Team B runs: ";
        cin >> runsB;
        teamA += runsA;
        teamB += runsB;
    }

    // Display final output
    cout << endl << "----- Final Score -----" << endl;
    cout << "Team A: " << teamA << endl;
    cout << "Team B: " << teamB << endl;

    // Announce winner or tie
    if (teamA > teamB)
        cout << "Team A wins!" << endl;
    else if (teamB > teamA)
        cout << "Team B wins!" << endl;
    else
        cout << "It's still a tie game!" << endl;

    return 0;
}

/* 
==================
Execution Results
==================
Team A vs Team B Score Counter
--------------------------------
Inning 1 - Team A runs: 5
Inning 1 - Team B runs: 4
Inning 2 - Team A runs: 4
Inning 2 - Team B runs: 4
Inning 3 - Team A runs: 7
Inning 3 - Team B runs: 5
Inning 4 - Team A runs: 5
Inning 4 - Team B runs: 7
Inning 5 - Team A runs: 0 
Inning 5 - Team B runs: 0
Inning 6 - Team A runs: 4
Inning 6 - Team B runs: 4
Inning 7 - Team A runs: 0
Inning 7 - Team B runs: 0
Inning 8 - Team A runs: 0
Inning 8 - Team B runs: 0
Inning 9 - Team A runs: 1
Inning 9 - Team B runs: 1

----- Final Score -----
Team A: 26
Team B: 25
Team A wins!
*/