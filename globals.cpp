/*

 tinymudserver - an example MUD server

 Author:  Nick Gammon 
          http://www.gammon.com.au/ 

(C) Copyright Nick Gammon 2004. Permission to copy, use, modify, sell and
distribute this software is granted provided this copyright notice appears
in all copies. This software is provided "as is" without express or implied
warranty, and with no claim as to its suitability for any purpose.
 
*/

// standard library includes ...

using namespace std; 

#include "globals.h"

// global variables
bool   bStopNow = false;      // when set, the MUD shuts down
bool   firstRun = true;       // boolean for first time run, for generating random seed
time_t tLastMessage = 0;      // time we last sent a periodic message 
time_t tLastDeathCheck = 0;   // time since last death check
time_t tLastMonsterMove = 0;  // time since last monster movement
time_t tLastMonsterSpawn = 0; // time since last monster spawn
int    maxMonNum = 0;         // current largest monster vnum, for spawning new mons
int    iControl = NO_SOCKET;  // socket for accepting new connections 


//list of all monsters
monsterList monmap;
// all monster names
vector<string> monnames;
// all valid room numbers
vector<int> roomnums;
// list of all connected players
tPlayerList playerlist;
// map of all rooms
tRoomMap roommap;
// map of known commands (eg. look, quit, north etc.)
map<string, tHandler> commandmap;
// map of things to do for various connection states
map<tConnectionStates, tHandler> statemap;
// messages
map<string, string, ciLess> messagemap;
// directions
set<string, ciLess> directionset;
// bad player names
set<string, ciLess> badnameset;
// blocked IP addresses
set<string> blockedIP;
