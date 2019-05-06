//
//  monster.hpp
//  ULMUD
//
//  Created by Ben Anding on 9/11/18.
//

#ifndef ITNYMUD_MONSTER_H
#define TINYMUD_MONSTER_H


#include <set>
#include <list>

#include "strings.h"

class monster
{
public:
    int mnum;         // Object's Number
    int room;
    int monhealth;
    bool immobile; // Whether the monster can move
    std::string behavior; // Whether the monster is hostile or friendly
    std::string name; // Name of Object
    
    //constructor
    monster (int mnum1, int room1, int monhealth1, std::string name1) : mnum (mnum1), room (room1), monhealth (monhealth1),  name (name1) {}

    void Save ();
    void MonsterMove (monster * m, const std::string & direction);
    void MonsterToRoom (monster * m,       // which player
                        const int & vnum,   // which room
                        const std::string & sOthersDepartMessage,  // tell people in original room
                        const std::string & sOthersArrriveMessage);
    void MonsterSendToAll (const std::string & message, const int InRoom);
};


typedef std::list <monster*> monsterList;
typedef monsterList::const_iterator monsterListIterator;

struct findMonsterName
{
    const std::string name;
    // ctor
    findMonsterName (const std::string & n) : name ( n ) {}
    // check for player with correct name, and actually playing
    bool operator() (const monster * m) const
    {
        return ciStringEqual (m->name, name);
    } // end of operator()
};  // end of findPlayerName


monster * FindMon (const int & vnum);



#endif TINYMUD_MONSTER_H //end TINYMUD_MONSTER_H
