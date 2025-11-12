// ================================================
// The Sea Siren Text Adventure Game
// Matt Johns - CISP 360
// ================================================
// This is a text-based adventure game where the player explores the wreck of the Sea Siren,
// a sunken cruise liner, encountering various rooms, items, and dangers.
// PQ: Passenger Quarters, HW: Hallway, SW: Stairwell, GB: Grand Ballroom,
// OQ: Officer's Quarters, OD: Observation Deck, BH: Ballast Hall,

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

// Greeting function
void ProgramGreeting() {
    // There maybe too much text here, it's not a graphic novel but I really enjoyed writing it.
    cout << "\n===========================================" << endl;
    cout << "    Exploring a Long Forgotten Watery Tomb" << endl;
    cout << "===========================================\n" << endl;
    cout << "You descend into the wreckage of the Sea Siren,\n"
         << "a once-luxurious cruise liner that mysteriously sunk on its maiden voyage.\n"
         << "The many lost souls of the passengers that once enjoyed the ship's many luxuries\n"
         << "now find themselves trapped in this watery grave site.\n"
         << "Down here, death awaits around every corner and secrets once thought lost forever\n"
         << "now wait to be discovered. Escape... if you can.\n" << endl;
    cout << "You slip through a open porthole in the side of the Sea Siren Luxury Cruise Liner..." << endl;
    cout << "As you enter through the opening the porthole slams shut behind you.\n"
         << "The silence is thick as you realize that there is no going back now.\n" << endl;
    cout << "Created by: Matt Johns\n" << endl;
}

// Inventory and item check
vector<string> inventory;
bool hasItem(const string& item) {
    for (const auto& i : inventory) if (i == item) return true; // linear search (ok small inv)
    return false; // not found
}

// Room structure (core to logic)
struct Room {
    int id;
    string name;
    string desc;
    string reentryDesc;
    vector<string> items;
    int north = -1, south = -1, east = -1, west = -1, up = -1, down = -1;
    bool visited = false;
    bool hasShark = false;
    bool isExit = false;
    bool isDHtoSWDoor = false; // block DH->SW until fixed
};

int main() {
    srand(time(0)); // rng for sharks (do at top)
    ProgramGreeting();

    string diverName;
    cout << "What's your name, diver? ";
    getline(cin, diverName);
    cout << "Welcome aboard, " << diverName << ".\n\n";

    // Room layout (had to map out visually on paper first)
    // Mapping out in 3D on paper w/ up & down is tough
    // Used a 2D array for connections.
    vector<Room> rooms = {
        // PQ: Knife is only revealed when player chooses to look
        // PQ is the starting room, and the only one without a shark chance
        // Knife is needed to survive shark encounters
        // GB: Coffee cup is required to stop door from closing in Dining Hall
        // DH: Crowbar and Nut & Bolt are needed to fix the broken door in OD to escape
        // OD: Map tells player where to exit, but only if they have the Ancient Artifact
        {0, "Passenger Quarters", "Water logged bunk beds, the merky waters of lost hope.",
            "Water logged bunk beds, the merkey waters of lost hope.", {"Knife"}},
        {1, "Hallway (PQ-HW)", "A narrow passage. There are stairs going up and stairs going down.",
            "A narrow passage. There are stairs going up and stairs going down.", {}},
        {2, "Stairwell", "The ship's main stairwell. Faint beams of light leak from above and below.",
            "The ship's main stairwell. faint beams of light leak from above and below.", {}},
        {3, "Grand Ballroom", "Slimy kelp drappes over everything including the grand piano.",
            "Slimy kelp drappes over everything including the grand piano.", {"Metal Coffee Cup"}},
        {4, "Hallway (GB-OQ)", "Portraits line the walls. The sea water feels heavy and oily.",
            "Portraits line the walls. The sea water feels heavy and oily.", {}},
        {5, "Officer's Quarters", "The officer's uniforms float about. On a table: A splintered wooden map.",
            "The officer's uniforms float about. A splintered wooden map.", {"Map Etched on Rotted Wood"}},
        {6, "Observation Deck", "slime and barnacles cover the glass. The surface world feels close.",
            "slime and barnacles cover the glass. The surface world feels close.", {}, -1,-1,-1,-1,-1,-1, true},
        {7, "Ballast Hall", "It's cold and dark here but oddly you can hear the sound of a piano playing. Weird.",
            "It's cold and dark here but oddly you can hear the sound of a piano playing. Weird.", {}},
        {8, "Cargo Hold", "Busted up crates are suspended in place. Something shinny in the corner.",
            "busted up crates are suspended in place. Something shinny in the corner.", {"Ancient Artifact"}},
        {9, "Engine Room", "Echoing clangs of machinery. Shadows flicker near the gears.",
            "Echoing clangs of machinery. Shadows flicker near the gears.", {"Crowbar", "Nut and Bolt"}},
        {10, "Hallway (ER-HW)", "Rusted pipes. A stairwell leads upward.",
            "Rusted pipes. A stairwell leads upward.", {}},
        {11, "Dining Hall", "Skeletal remains everywhere. A stairwell door to the north is barely hanging on its hinges.",
            "Skeletal remains everywhere. A stairwell door to the north is barely hanging on its hinges.", {}, -1,-1,-1,-1,-1,-1,false,true},
        {12, "Stairwell (DH-OD)", "A final climb. Fresh water rushes through the cracks.",
            "A final climb. Fresh water rushes through the cracks.", {}}
    };

    // Connections
    rooms[0].east = 1;         // PQ to HW
    rooms[1].up = 3;           // HW to GB
    rooms[1].down = 7;         // HW to BH
    rooms[3].east = 4;         // GB to HW
    rooms[4].north = 5;        // HW to OQ
    rooms[5].south = 4;        // OQ to HW back
    rooms[4].west = 3;         // HW to GB back
    rooms[3].down = 1;         // GB down to HW
    rooms[7].east = 8;         // BH to CH
    rooms[8].east = 9;         // CH to ER
    rooms[9].east = 10;        // ER to HW
    rooms[10].up = 11;         // HW to DH
    rooms[11].north = 12;      // DH to SW (DH-OD)
    rooms[12].north = 6;       // SW to OD

    // Shark RNG - not in PQ
    for (size_t i = 1; i < rooms.size(); ++i) {
        rooms[i].hasShark = (rand() % 3 == 0); // 1/3 chance per room (not PQ)
    }

    int currentRoom = 0;
    bool gameRunning = true;
    map<int, bool> roomLooked;      // tracks if player looked in each room
    map<int, bool> itemsRevealed;   // items can only be seen if looked
    string input;
    bool DHDoorFixed = false; // has the broken DH->SW door been fixed?

    // Shark logic variables
    bool roomLookedThisVisit = false;
    int previousRoom = -1;

    while (gameRunning) {
        Room& room = rooms[currentRoom];

        // Reset look-this-visit if entering new room
        if (currentRoom != previousRoom) {
            roomLookedThisVisit = false;
            previousRoom = currentRoom;
        }

        cout << "\nYou are in: " << room.name << endl;
        if (!room.visited) {
            cout << room.desc << endl;
            room.visited = true;
        } else {
            cout << room.reentryDesc << endl;
        }

        // Special Dining Hall door logic
        if (room.isDHtoSWDoor && !DHDoorFixed) {
            cout << "\nThe door to the stairwell is broken and blocks your path north." << endl;
            if (hasItem("Crowbar") && hasItem("Nut and Bolt")) {
                cout << "You could use your Crowbar and Nut and Bolt to fix it. Do so now? (Y/N): ";
                getline(cin, input);
                if (!input.empty() && toupper(input[0]) == 'Y') {
                    cout << "You fix the hinge with some clever improvisation. The path north is now open." << endl;
                    DHDoorFixed = true;
                } else {
                    cout << "You wait, the door is still blocking the way." << endl;
                }
            } else {
                cout << "You need a Crowbar and Nut and Bolt to fix the door." << endl;
            }
            rooms[11].north = DHDoorFixed ? 12 : -1;
        }

        // Check if command is a movement command
        auto isMoveCmd = [](char c) {
            return c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'U' || c == 'D';
        };

        cout << "\nCommand [N/S/E/W/U/D | L:Look | I:Inventory | Q:Quit]: ";
        getline(cin, input);
        if (input.empty()) continue;
        char cmd = toupper(input[0]);

        // Shark hazard: if player tries to move before looking in a shark room, instant death
        if (room.hasShark && !roomLookedThisVisit && isMoveCmd(cmd)) {
            cout << "You didn't check your surroundings. As you move, a shark ambushes you!\nGAME OVER." << endl;
            break;
        }

        switch (cmd) {
            case 'N':
                if (room.north != -1) currentRoom = room.north;
                else cout << "Can't go north." << endl;
                break;
            case 'S':
                if (room.south != -1) currentRoom = room.south;
                else cout << "Can't go south." << endl;
                break;
            case 'E':
                if (room.east != -1) currentRoom = room.east;
                else cout << "Can't go east." << endl;
                break;
            case 'W':
                if (room.west != -1) currentRoom = room.west;
                else cout << "Can't go west." << endl;
                break;
            case 'U':
                if (room.up != -1) currentRoom = room.up;
                else cout << "Can't go up." << endl;
                break;
            case 'D':
                if (room.down != -1) currentRoom = room.down;
                else cout << "Can't go down." << endl;
                break;
            case 'L': {
                // Only show items if room hasn't been looted and player looks
                if (room.items.empty() || itemsRevealed[room.id]) {
                    cout << "Nothing has changed about the room." << endl;
                } else {
                    cout << "You notice: ";
                    for (const string& item : room.items) cout << item << ", ";
                    cout << "\nPick up items? (Y/N): ";
                    getline(cin, input);
                    if (!input.empty() && toupper(input[0]) == 'Y') {
                        for (const string& item : room.items) {
                            inventory.push_back(item);
                            cout << "Picked up: " << item << endl;
                        }
                        room.items.clear();
                        cout << "Items have been added to your inventory." << endl; // Confirm item pickup, don't show them in description anymore
                    }
                    itemsRevealed[room.id] = true;
                }
                // Shark check ON look (gives player fair shot to kill)
                if (room.hasShark) {
                    if (hasItem("Knife")) {
                        cout << "A shark appears! You slash with your knife—it vanishes into the gloom." << endl;
                        room.hasShark = false;
                    } else {
                        cout << "A shark appears—and you have nothing to defend yourself!\nGAME OVER." << endl;
                        gameRunning = false;
                    }
                }
                roomLooked[room.id] = true;
                roomLookedThisVisit = true;
                break;
            }
            case 'I': {
                cout << "\nInventory: ";
                if (inventory.empty()) cout << "Empty." << endl;
                else {
                    for (const string& i : inventory) cout << i << ", ";
                    cout << endl;
                }
                break;
            }
            case 'Q':
                cout << "You abandon the Sea Siren..." << endl;
                gameRunning = false;
                break;
            default:
                cout << "Invalid input." << endl;
        }

        // Exit win logic
        if (room.isExit) {
            if (hasItem("Map Etched on Rotted Wood") && hasItem("Ancient Artifact")) {
                cout << "\nYou follow the map, clutching the artifact, and break for the light—escape!" << endl;
                gameRunning = false;
            } else {
                cout << "\nYou reach the surface but can't open the hatch without the map and artifact." << endl;
            }
        }
    }

    return 0;
}

/*
==========================================
Projected Outcome (for grading or personal ref)
==========================================
- Player starts in PQ. Can't return. Must explore.
- 'Look' in PQ = find knife.
- Player moves through hallways, stairwells, GB, finds coffee cup, etc.
- Items are only shown when LOOK is used and never before.
- Sharks randomly spawn. Player only dies to shark if they fail to LOOK before acting (see above).
- Special item logic for: fixing door, map, artifact.
- Only way to win: have both artifact AND map before reaching Observation Deck.
- Plenty of inline comments and unique notes for my own debugging.
*/

