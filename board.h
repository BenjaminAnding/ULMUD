//
//  board.hpp
//  ULMUD
//
//  Created by Ben Anding on 9/11/18.
//

#ifndef TINYMUD_BOARD_H
#define TINYMUD_BOARD_H

#include <sstream>
#include <iostream>
#include <set>
#include <list>
#include <map>
#include "strings.h"
#include "unistd.h"

class board
{
public:
    int bnum;         // Object's Number
    int room;
	std::string lastpost;
	std::string inbuf;
	int * user;
    bool inuse; // Whether the board can move
    std::map<std::string, std::string> contents;
    std::string bname; // Name of Object
    
    //constructor
    board (int bnum1, int room1, std::string bname1) : bnum (bnum1), room (room1), bname (bname1) {}
    void load ();
	std::string leaf (int lower, int upper);
	std::string post (std::istream & sArgs);
	void remove (int which);
	std::string boardread (int which);
    void save ();
};

typedef std::list <board*> boardList;
typedef boardList::const_iterator boardListIterator;

struct findBoardName
{
    const std::string name;
    // ctor
    findBoardName (const std::string & n) : name ( n ) {}
    // check for player with correct name, and actually playing
    bool operator() (const board * m) const
    {
        return ciStringEqual (m->bname, name);
    } // end of operator()
};  // end of findPlayerName


board * FindBoard (const int & vnum);

#endif TINYMUD_BOARD_H //end TINYMUD_BOARD_H
