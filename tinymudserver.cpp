/*

 tinymudserver - an example MUD server

 Author:  Nick Gammon 
          http://www.gammon.com.au/ 

 Date:    22nd July 2004

(C) Copyright Nick Gammon 2004. Permission to copy, use, modify, sell and
distribute this software is granted provided this copyright notice appears
in all copies. This software is provided "as is" without express or implied
warranty, and with no claim as to its suitability for any purpose.
 
*/

// standard library includes ...

#include <iostream>

using namespace std; 

#include "constants.h"
#include "globals.h"

void LoadThings ();
int InitComms ();
void MainLoop ();
void CloseComms ();

// called approximately every 0.5 seconds - handle things like fights here
void PeriodicUpdates ()
{
    //      The example below just sends a message every MESSAGE_INTERVAL seconds.
    // send new command if it is time
  if (time (NULL) > (tLastMessage + MESSAGE_INTERVAL))
    {
    SendToAll ("Autosaving characters...\r\n");
        for(tPlayerListIterator i; i == playerlist.end(); i++) {
            tPlayer * p = *i;
            p->Save();
        }

    tLastMessage = time (NULL);

    }

     // move monsters every 10-15 seconds
  if (time (NULL) > (tLastMonsterMove + rand() % 15 + 10))
    {
    std::string arrayNum[4] = {"n", "e", "s", "w"};
    for (monsterListIterator listiter1 = monmap.begin ();
         listiter1 != monmap.end ();
         listiter1++)
    {
        int RandIndex = rand() % 4;
        monster *m = *listiter1;
        m->MonsterMove(m, arrayNum[RandIndex]);
    } // end of looping through all monsters

    tLastMonsterMove = time (NULL);

    }
  if (time (NULL) > tLastDeathCheck)
    {
        for (tPlayerListIterator i = playerlist.begin(); i != playerlist.end(); i++) {
            tPlayer * p = *i;
            if (p->health <= 0)
            {
                p->flags.insert("dead");
            }
            p->DeathFunction();
        }
        tLastDeathCheck = time (NULL);
    }
	    

    // if the server is not empty
    // spawn a monster in a random room once every minute
  if (!playerlist.empty() && time (NULL) > (tLastMonsterSpawn + MONSTER_SPAWN_INTERVAL) && maxMonNum <= 99)
    {        
        if (firstRun)
        {
			board * b = new board(1, 1000, "main");
			b->load();
			bmap.push_back(b);
            srand(time (NULL)); // seed for random value
            monnames.erase(monnames.end() - 1); // get rid of empty monname at end
            firstRun = false;
        }

        // find a random room & monster name
        int randRoomNum = roomnums.front() + rand() % ((roomnums.back() + 1) - roomnums.front());
        int randNameNum = rand() % monnames.size();
        std::string randName = monnames.at(randNameNum); // get the name of the rand monster to spawn
        int monHealth = healthmap.find(randName)->second; // get the health value corresponding to the rand monster

        // create a new random monster in a random room, increase its vnum
        maxMonNum++;
        monster * m = new monster(maxMonNum, randRoomNum, monHealth, randName);
        monmap.push_back(m);

        tLastMonsterSpawn = time (NULL);
    }
  
  if (time (NULL) > (tLastMonsterHealthCheck))
    {
        for (monsterListIterator listiter1 = monmap.begin (); listiter1 != monmap.end ();
            listiter1++)
        {
            monster *m = *listiter1;
            if (m->monhealth <=0 )
            {
                SendToAll("\r\n"+m->name+" has perished.\r\n> ");
        		tPlayer *aggressor = (tPlayer*)m->aggressor;
                listiter1 = monmap.erase(listiter1); // or before C++11 monmap.erase(listiter1++);
				aggressor->score = aggressor->score + 1;
				maxMonNum--;
            }
        }
    }
  
  // beginning of monster attack
  if (time (NULL) > (tLastMonsterAttack + 3))
  {
    // check for monsters with hostile behavior
    for (monsterListIterator listiter1 = monmap.begin ();
         listiter1 != monmap.end ();
         listiter1++)
    {
        monster *hostileMon = *listiter1;
        tPlayer *aggressor = (tPlayer*)hostileMon->aggressor;
        if (hostileMon->behavior == "hostile")
        {     
            for (tPlayerListIterator i = playerlist.begin(); 
                i != playerlist.end(); 
                i++) 
            {
                tPlayer * p = *i;
				if (p == aggressor) 
				{
                	if (hostileMon->room == p->room) // check for hostile mons in the same room as players
                		{
                    		*p << hostileMon->name << " swipes at you, dealing " << BASE_MON_DAMAGE << " damage.\r\n";
                    		p->health = p->health - 3;
                		}
                	else
                	{
                    	hostileMon->behavior = "neutral"; // if room is empty, reset monster's behavior
                    	hostileMon->immobile = false;
                	}
            	}
        	}
		}
        tLastMonsterAttack = time (NULL);
    }

  }

} // end of PeriodicUpdates


// main program
int main ()
{
  cout << "Tiny MUD server version " << VERSION << endl;

  LoadThings ();    // load stuff
  
  if (InitComms ()) // listen for new connections
    return 1;

  cout << "Accepting connections from port " <<  PORT << endl;
  
  MainLoop ();    // handle player input/output

  // game over - tell them all
  SendToAll ("\r\n\r\n** Game shut down. **\r\n\r\n");
  
  CloseComms ();  // stop listening

  cout << "Game shut down." << endl;  
  return 0;
}   // end of main
