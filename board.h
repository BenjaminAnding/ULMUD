//
//  board.hpp
//  ULMUD
//
//  Created by Ben Anding on 9/11/18.
//

#ifndef ITNYMUD_BOARD_H
#define TINYMUD_BOARD_H


#include <set>
#include <list>

#include "strings.h"

class board
{
public:
    int mnum;         // Object's Number
    int room;
	int * user;
    bool inuse; // Whether the board can move
    
    
    //constructor
    board (int mnum1, int room1) : mnum (mnum1), room (room1) {}

    void Save ();
    void BoardMove (board * m, const std::string & direction);

typedef std::list <board*> boardList;
typedef boardList::const_iterator boardListIterator;

board * FindMon (const int & vnum);



#endif TINYMUD_BOARD_H //end TINYMUD_BOARD_H
