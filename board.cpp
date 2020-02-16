//
//  board.cpp
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

/*void board::Save ()
{
    std::ifstream fMon (MONSTERS_FILE, std::ios::in);
    if (!fMon)
    {
        std::cerr << "Could not open objects file: " << MONSTERS_FILE << std::endl;
        return;
    }
    std::vector<std::string> boards;
    while (!(fMon.eof ()))
    {
        
        int mnum1;
        int room1;
        std::string monname;
        fMon >> mnum1;
        if (mnum != mnum1) {
            boards.push_back(std::to_string(mnum1));
        //std::cerr << vnum << std::endl;
            fMon >> room1;
            boards.push_back(std::to_string(room));
        //std::cerr << room << std::endl;
            fMon >> monname;
            boards.push_back(monname);
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
    for (std::vector<std::string>::iterator listiter = boards.begin ();
         listiter != boards.end ();
         listiter++) {
        f << *listiter;
    }
} /* end of board::Save */

struct boardSendToPlayer
{
    const std::string message;
    const int room;
    
    // ctor
    boardSendToPlayer (const std::string & m, const int r = 0)
    : message (m), room (r) {}
    // send to this player
    void operator() (tPlayer * p)
    {
        if (p->IsPlaying () && (room == 0 || p->room == room))
            *p << message;
    } // end of operator()
};  // end of boardSendToPlayer
