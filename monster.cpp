//
//  monster.cpp
//  ULMUD
//
//  Created by Ben Anding on 9/11/18.
//

#include "utils.h"
#include "globals.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

monster * FindMonster (const std::string & name)
{
    monsterListIterator i =
    std::find_if (monmap.begin (), monmap.end (), findMonsterName (name));
    
    if (i == monmap.end ())
        return NULL;
    else
        return *i;
} //end of FindMonster

/*void monster::Save ()
{
    std::ifstream fMon (MONSTERS_FILE, std::ios::in);
    if (!fMon)
    {
        std::cerr << "Could not open objects file: " << MONSTERS_FILE << std::endl;
        return;
    }
    std::vector<std::string> monsters;
    while (!(fMon.eof ()))
    {
        
        int mnum1;
        int room1;
        std::string monname;
        fMon >> mnum1;
        if (mnum != mnum1) {
            monsters.push_back(std::to_string(mnum1));
        //std::cerr << vnum << std::endl;
            fMon >> room1;
            monsters.push_back(std::to_string(room));
        //std::cerr << room << std::endl;
            fMon >> monname;
            monsters.push_back(monname);
        //std::getline (fMon, monname);
        //std::cerr << monname << std::endl;
        }
    }
    std::ofstream f (MONSTERS_FILE, std::ios::out);
    if (!f)
    {
        std::cerr << "Could not write to file." << std::endl;
        return;
    }
    
    // write player details
    f << mnum << std::endl;
    f << room << std::endl;
    f << name << std::endl;
    for (std::vector<std::string>::iterator listiter = monsters.begin ();
         listiter != monsters.end ();
         listiter++) {
        f << *listiter;
    }
} /* end of monster::Save */


void monster::MonsterMove (monster * m, const std::string & direction)
{
    // get current room (throws exception if not there)
    tRoom * r = FindRoom (m->room);
    
    // find the exit
    tExitMap::const_iterator exititer = r->exits.find (direction);
    
    if (exititer == r->exits.end ())
    {
        ;
    }
    
    // move player
    monster::MonsterToRoom (m, exititer->second,
                   m->name + " goes " + direction + "\r\n",
                   m->name + " enters.\r\n");
    
} // end of monsterMove

void monster::MonsterToRoom (monster * m,       // which player
                    const int & vnum,   // which room
                             const std::string & sOthersDepartMessage,  // tell people in original room
                             const std::string & sOthersArrriveMessage) // tell people in new room
{
    tRoom * r = FindRoom (vnum); // find the destination room (throws exception if not there)
    MonsterSendToAll (sOthersDepartMessage, m->room);  // tell others where s/he went
    m->room = vnum;  // move to new room
    MonsterSendToAll (sOthersArrriveMessage, m->room);  // tell others ws/he has arrived
} // end of monsterToRoom

struct monsterSendToPlayer
{
    const std::string message;
    const int room;
    
    // ctor
    monsterSendToPlayer (const std::string & m, const int r = 0)
    : message (m), room (r) {}
    // send to this player
    void operator() (tPlayer * p)
    {
        if (p->IsPlaying () && (room == 0 || p->room == room))
            *p << message;
    } // end of operator()
};  // end of monsterSendToPlayer

void monster::MonsterSendToAll (const std::string & message, const int InRoom)
{
    std::for_each (playerlist.begin (), playerlist.end (),
              monsterSendToPlayer (message, InRoom));
} /* end of MonsterSendToAll */

