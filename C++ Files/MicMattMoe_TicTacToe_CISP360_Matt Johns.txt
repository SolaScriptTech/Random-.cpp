// MicMattMoe_TicTacToe.cpp
// Matt Johns, CISP 360
// July 28, 2025

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// greet + req stuff
void ProgramGreeting() {
    cout << "Welcome to MicMattMoe!" << endl;
    cout << "Author: Matt Johns" << endl;
    cout << "Due Date: July 28, 2025" << endl;
}

// draw board, basic af
void DisplayBoard(const char board[]) {
    cout << "\n";
    for (int i = 0; i < 9; i++) {
        cout << " " << board[i] << " ";
        if ((i + 1) % 3 == 0)
            cout << "\n";
        else
            cout << "|"; // so I don't forget the dividers
    }
    cout << "\n";
}

// flip rng to pick starter
bool HumanGoesFirst() {
    srand(time(0)); // don't move this or rand will repeat
    return rand() % 2 == 0;
}

// valid sq? check it
bool IsValidMove(const char board[], int move) {
    return move >= 1 && move <= 9 && board[move - 1] != 'M' && board[move - 1] != 'O'; // sanity gate
}

// user input loop w/ filter
void HumanTurn(char board[]) {
    int move;
    do {
        cout << "Enter your move (1-9): ";
        cin >> move;

        if (!cin || !IsValidMove(board, move)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid move. Try again.\n";
        }
    } while (!IsValidMove(board, move));

    board[move - 1] = 'M'; // lock move in
}

// dumb AI, rand guesser
void ComputerTurn(char board[]) {
    int move;
    do {
        move = rand() % 9; // keeping this dumb on purpose
    } while (!IsValidMove(board, move + 1));

    board[move] = 'O';
    cout << "Not so good computer chose position " << (move + 1) << "\n";
}

// win logic - check rows, cols, diag
char CheckWinner(const char board[]) {
    for (int i = 0; i < 9; i += 3) {
        if (board[i] == board[i+1] && board[i] == board[i+2]) return board[i];
    }
    for (int i = 0; i < 3; i++) {
        if (board[i] == board[i+3] && board[i] == board[i+6]) return board[i];
    }
    if (board[0] == board[4] && board[0] == board[8]) return board[0]; // TL to BR diag
    if (board[2] == board[4] && board[2] == board[6]) return board[2]; // TR to BL diag

    return ' '; // no winner yet
}

// ask to replay (y/n)
bool AskReplay() {
    char choice;
    do {
        cout << "Do you wish to waste more time and play again? (y/n): ";
        cin >> choice;
        choice = tolower(choice);
        if (choice != 'y' && choice != 'n') {
            cout << "Sudo select 'y' or 'n'.\n";
        }
    } while (choice != 'y' && choice != 'n');

    return choice == 'y'; // short-circuit logic
}

// menu select w/ guard
int ShowMenu() {
    int choice;
    do {
        cout << "\n--- Matt's Menu ---\n";
        cout << "1. Learn How To Play\n";
        cout << "2. Let's Get Playing\n";
        cout << "3. I Want To Quit\n";
        cout << "Make Your Selection: ";
        cin >> choice;

        if (!cin || choice < 1 || choice > 3) {
            cin.clear();
            cin.ignore(10000, '\n'); // clear out junk input
            cout << "That selection is no good. You must enter 1, 2, or 3.\n";
        }
    } while (choice < 1 || choice > 3);

    return choice;
}

int main() {
    ProgramGreeting();

    int choice = ShowMenu();

    if (choice == 1) {
        // explainer for noobs
        cout << "\nMicMattMoe is made for just two-players. One lucky player is M, the other not so lucky player is O.\n";
        cout << "The first player to get three like characters in a row wins (horizontal, vertical, or diagonal).\n";
        cout << "You'll play against a \"not so good computer\" using worse than random selections.\n";
        choice = ShowMenu(); // jump back to menu
    }

    if (choice == 3) {
        // peace out
        cout << "Thanks for trying MicMattMoe. Now go do something productive!\n";
        return 0; // early out
    }

    bool playAgain;

    do {
        // empty board
        char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }; // keeping it clean for round 2

        DisplayBoard(board);

        bool humanTurn = HumanGoesFirst(); // flip it each game

        if (humanTurn)
            cout << "Lucky player M goes first!\n";
        else
            cout << "Not so lucky player O (Not so good computer) goes first!\n";

        char winner = ' ';
        int turns = 0;

        // loop til win or full
        do {
            DisplayBoard(board); // show current state

            if (humanTurn) {
                HumanTurn(board);
            } else {
                ComputerTurn(board);
            }

            humanTurn = !humanTurn; // toggle turn
            turns++;

            winner = CheckWinner(board); // check after each move

        } while (winner == ' ' && turns < 9);

        DisplayBoard(board);

        if (winner == 'M')
            cout << "BAZINGA! Lucky player M wins!\n"; // winner msg
        else if (winner == 'O')
            cout << "Not so good computer wins! You should be ashamed of yourself.\n"; // loser msg
        else
            cout << "Ahh snap, it's a tie!\n";  // tie

        playAgain = AskReplay(); // loop decision

    } while (playAgain); // replay loop

    cout << "Thanks for trying out MicMattMoe!\n";
    return 0; // wrap it
}

/*
========================================
||         POTENTIAL OUTCOMES         ||
========================================
- Player M wins --> "BAZINGA! Lucky player M wins!"
- Player O wins --> "Not so good computer wins! You should be ashamed of yourself."
- Tie game      --> "Ahh snap, it's a tie!"
- Quit early    --> "Thanks for trying MicMattMoe. Now go do something productive!"
- Replay loop   --> Prompts: "Do you wish to waste more time and play again? (y/n):"
*/
