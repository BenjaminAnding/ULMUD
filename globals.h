#include <set>
#include <string>
#include <map>
#include <vector>

#include "monster.h"  // for monster list
#include "player.h"   // for player list
#include "room.h"     // for rooms and exits

// list of all monsters
extern monsterList monmap;
// list of monster names
extern std::vector<std::string> monnames;
// list of valid room numbers
extern std::vector<int> roomnums;
// bad player names
extern std::set<std::string, ciLess> badnameset;
// blocked IP addresses
extern std::set<std::string> blockedIP;
// list of all connected players
extern tPlayerList playerlist;
// map of all rooms
extern tRoomMap roommap;
// map of known commands (eg. look, quit, north etc.)
extern std::map<std::string, tHandler> commandmap;
// map of things to do for various connection states
extern std::map<tConnectionStates, tHandler> statemap;
// messages
extern std::map<std::string, std::string, ciLess> messagemap;
// map of monster types and their health values
extern std::map<std::string, int> healthmap;
// directions
extern std::set<std::string, ciLess> directionset;


// global variables
extern bool   bStopNow;      // when set, the MUD shuts down
extern bool   firstRun;  // boolean for first time run, for generating rand seed
extern time_t tLastMessage;      // time we last sent a periodic message 
extern time_t tLastDeathCheck;    // time since last death check
extern int    iControl;  // socket for accepting new connections
extern int    maxMonNum; // current largest monster vnum, for spawning new mons
extern time_t tLastMonsterMove; // time since last monster movement
extern time_t tLastMonsterSpawn; // time since last monster spawn
extern time_t tLastMonsterHealthCheck; // time since last monster health check
extern time_t tLastMonsterAttack; // time since last time monster attacked
