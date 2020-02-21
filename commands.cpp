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

#include <stdexcept>
#include <iostream>

using namespace std; 

#include "utils.h"
#include "constants.h"
#include "strings.h"
#include "player.h"
#include "room.h"
#include "globals.h"

void NoMore (tPlayer * p, istream & sArgs)
  {
  string sLine;
  getline (sArgs, sLine);
  if (!sLine.empty ())
    throw runtime_error ("Unexpected input: " + sLine);
  } // end of NoMore

// helper function for say, tell, chat, etc.
string GetMessage (istream & sArgs, const string & noMessageError)
  {
  string message;
  sArgs >> ws;  // skip leading spaces
  getline (sArgs, message); // get rest of line
  if (message.empty ()) // better have something
    throw runtime_error (noMessageError);
  return message;  
  } // end of GetMessage
  
// helper function for get a flag
string GetFlag (istream & sArgs, const string & noFlagError)
  {
  string flag;
  sArgs >> ws >> flag;
  if (flag.empty ())
    throw runtime_error (noFlagError);
  if (flag.find_first_not_of (valid_player_name) != string::npos)
    throw runtime_error ("Flag name not valid.");
  return flag;      
  } // end of GetFlag 

void PlayerToRoom (tPlayer * p,       // which player
                  const int & vnum,   // which room
                  const string & sPlayerMessage,  // what to tell the player
                  const string & sOthersDepartMessage,  // tell people in original room 
                  const string & sOthersArriveMessage) // tell people in new room
{
  tRoom * r = FindRoom (vnum); // find the destination room (throws exception if not there)
  SendToAll (sOthersDepartMessage, p, p->room);  // tell others where s/he went
  p->room = vnum;  // move to new room
  *p << sPlayerMessage; // tell player
  SendToAll (sOthersArriveMessage, p, p->room);  // tell others ws/he has arrived  
  p->DoCommand ("look");   // look around new room  
} // end of PlayerToRoom

void DoDirection (tPlayer * p, const string & sArgs)
  {
  // get current room (throws exception if not there)
  tRoom * r = FindRoom (p->room);

  // find the exit
 tExitMap::const_iterator exititer = r->exits.find (sArgs);    

  if (exititer == r->exits.end ())
    throw runtime_error ("You cannot go that way.");

  // move player
  PlayerToRoom (p, exititer->second,
                "You go " + sArgs + "\r\n",
                p->playername + " goes " + sArgs + "\r\n",
                p->playername + " enters.\r\n");
  
  } // end of DoDirection
  
/* quit */

void DoQuit (tPlayer * p, istream & sArgs)
  {
  NoMore (p, sArgs);  // check no more input
    
  /* if s/he finished connecting, tell others s/he has left */
  
  if (p->connstate == ePlaying)
    {
    *p << "See you next time!\r\n";
    cout << "Player " << p->playername << " has left the game.\r\n";
    SendToAll ("Player " + p->playername + " has left the game.\r\n", p);   
    } /* end of properly connected */

  p->ClosePlayer ();
  } // end of DoQuit

void lookObject (tPlayer * p, string & which)
  {
  *p << "Looking at object " << which << "\r\n";

  // scan available objects and display information about them ...
  }  // end of lookObject

void DoWait (tPlayer * p, istream & sArgs)
{
    *p << "Waiting..." << "\r\n";
}

/* look */

void DoLook (tPlayer * p, istream & sArgs)
{
 
  // TODO: add: look (thing)

  string whichObject;
  sArgs >> ws >> whichObject;

  if (!whichObject.empty ())
    {
    lookObject (p, whichObject);
    return;
    }
  
  // find our current room, throws exception if not there
  tRoom * r = FindRoom (p->room);
  
  // show room description
  *p << r->description;
  
  // show available exits
  if (!r->exits.empty ())
    {
    *p << "Exits: ";
    for (tExitMap::const_iterator exititer = r->exits.begin ();
         exititer != r->exits.end (); ++exititer)
      *p << exititer->first << " ";
    *p << "\r\n";        
    }
  
  /* list other players in the same room */
  
  int iOthers = 0;
  for (tPlayerListIterator listiter = playerlist.begin (); 
      listiter != playerlist.end (); 
      listiter++)
    {
    tPlayer *otherp = *listiter;
    if (otherp != p && /* we don't see ourselves */
        otherp->IsPlaying () &&
        otherp->room == p->room)  // need to be in same room
      {
      if (iOthers++ == 0)
        *p << "You also see ";
      else
        *p << ", ";
      *p << otherp->playername;
      }
    }   /* end of looping through all players */

  /* If we listed anyone, finish up the line with a period, newline */
    if (iOthers){
    *p << ".\r\n";
    }
    
    int iMonsters = 0;
    for (monsterListIterator listiter1 = monmap.begin ();
     listiter1 != monmap.end ();
     listiter1++)
    {
        monster *m = *listiter1;
        if (m->room == p->room)  // need to be in same room
        {
            if (iMonsters++ == 0)
                *p << "You also see ";
            else
                *p << ", ";
            *p << m->name;
        }
    }   /* end of looping through all players */
    
    /* If we listed anyone, finish up the line with a period, newline */
    if (iMonsters){
        *p << ".\r\n";
    }

  
} // end of DoLook

/* say <something> */

void DoSay (tPlayer * p, istream & sArgs)
{
  p->NeedNoFlag ("gagged"); // can't if gagged
  string what = GetMessage (sArgs, "Say what?");  // what
  *p << "You say, \"" << what << "\"\r\n";  // confirm
  SendToAll (p->playername + " says, \"" + what + "\"\r\n", 
            p, p->room);  // say it
} // end of DoSay 

/* tell <someone> <something> */

void DoTell (tPlayer * p, istream & sArgs)
{
  p->NeedNoFlag ("gagged"); // can't if gagged
  tPlayer * ptarget = p->GetPlayer (sArgs, "Tell whom?", true);  // who
  string what = GetMessage (sArgs, "Tell " + ptarget->playername + " what?");  // what  
  *p << "You tell " << ptarget->playername << ", \"" << what << "\"\r\n";     // confirm
  *ptarget << p->playername << " tells you, \"" << what << "\"\r\n";    // tell them
} // end of DoTell

void DoSave  (tPlayer * p, istream & sArgs)
{
  p->Save ();
  *p << "Saved.\r\n";  
}

void DoChat (tPlayer * p, istream & sArgs)
{
  p->NeedNoFlag ("gagged"); // can't if gagged
  string what = GetMessage (sArgs, "Chat what?");  // what  
  SendToAll (p->playername + " chats, \"" + what + "\"\r\n");  // chat it
}

void DoEmote (tPlayer * p, istream & sArgs)
{
  string what = GetMessage (sArgs, "Emote what?");  // what  
  SendToAll (p->playername + " " + what + "\r\n", 0, p->room);  // emote it
}

void DoWho (tPlayer * p, istream & sArgs)
{
  NoMore (p, sArgs);  // check no more input
  *p << "Connected players ...\r\n";
  
  int count = 0;
  for (tPlayerListIterator iter = playerlist.begin ();
       iter != playerlist.end ();
       ++iter)
    {
    tPlayer * pTarget = *iter;    // the player  
    if (pTarget->IsPlaying ())
      {
      *p << "  " << pTarget->playername << 
            " in room " << pTarget->room << "\r\n";
      ++count;
      } // end of if playing
    } // end of doing each player
  
  *p << count << " player(s)\r\n";  
} // end of DoWho

void DoSetFlag (tPlayer * p, istream & sArgs)
{
  p->NeedFlag ("can_setflag");  // permissions
  tPlayer * ptarget = p->GetPlayer (sArgs, "Usage: setflag <who> <flag>");  // who
  string flag = GetFlag (sArgs, "Set which flag?"); // what
  NoMore (p, sArgs);  // check no more input
  if (ptarget->flags.find (flag) != ptarget->flags.end ())    // check not set
    throw runtime_error ("Flag already set.");
  
  ptarget->flags.insert (flag);   // set it
  *p << "You set the flag '" << flag << "' for " << ptarget->playername << "\r\n";  // confirm
      
} // end of DoSetFlag

void DoClearFlag (tPlayer * p, istream & sArgs)
{
  p->NeedFlag ("can_setflag");  // permissions
  tPlayer * ptarget = p->GetPlayer (sArgs, "Usage: clearflag <who> <flag>");  // who
  string flag = GetFlag (sArgs, "Clear which flag?"); // what
  NoMore (p, sArgs);  // check no more input
  if (ptarget->flags.find (flag) == ptarget->flags.end ())    // check set
    throw runtime_error ("Flag not set.");

  ptarget->flags.erase (flag);    // clear it
  *p << "You clear the flag '" << flag << "' for " << ptarget->playername << "\r\n";  // confirm
      
} // end of DoClearFlag

void DoShutdown (tPlayer * p, istream & sArgs)
{
  NoMore (p, sArgs);  // check no more input
  p->NeedFlag ("can_shutdown");
  SendToAll (p->playername + " shuts down the game\r\n");
  bStopNow = true;
} // end of DoShutdown

void DoHelp (tPlayer * p, istream & sArgs)
{
  NoMore (p, sArgs);  // check no more input
  *p << messagemap ["help"];
} // end of DoHelp

void DoGoTo (tPlayer * p, istream & sArgs)
  {
  p->NeedFlag ("can_goto");

  int room;
  sArgs >> room;
  
  // check room number supplied OK
  if (sArgs.fail ())
    throw runtime_error ("Go to which room?");

  NoMore (p, sArgs);  // check no more input

  // move player
  PlayerToRoom (p, room,
                MAKE_STRING ("You go to room " << room << "\r\n"),
                p->playername + " disappears in a puff of smoke!\r\n",
                p->playername + " appears in a puff of smoke!\r\n");
  
  } // end of DoGoTo
  
void DoTransfer (tPlayer * p, istream & sArgs)
{
  p->NeedFlag ("can_transfer");  // permissions
  tPlayer * ptarget = p->GetPlayer (sArgs, 
    "Usage: transfer <who> [ where ] (default is here)", true);  // who
  int room;
  sArgs >> room;
  
  if (sArgs.fail ())
    room = p->room;   // if no room number, transfer to our room
  
  NoMore (p, sArgs);  // check no more input  

  *p << "You transfer " <<  ptarget->playername << " to room " << room << "\r\n";
  
   // move player
  PlayerToRoom (ptarget, room,
                p->playername + " transfers you to another room!\r\n",
                ptarget->playername + " is yanked away by unseen forces!\r\n",
                ptarget->playername + " appears breathlessly!\r\n");
     
} // end of DoTransfer

void DoKick (tPlayer * p, istream & sArgs)
{
    p->NeedFlag ("can_kick");
    tPlayer * ptarget = p->GetPlayer (sArgs, "Usage: kick <who>");
    NoMore (p, sArgs);  // check no more input
    SendToAll (p->playername + " kicked "+ptarget->playername+".\r\n");
    ptarget->ClosePlayer ();
}

/* process commands when player is connected */

void DoMonsterMove ()
{
    std::string arrayNum[4] = {"n", "e", "s", "w"};
    for (monsterListIterator listiter1 = monmap.begin ();
         listiter1 != monmap.end ();
         listiter1++)
    {
        int RandIndex = rand() % 4;
        monster *m = *listiter1;
        m->MonsterMove(m, arrayNum[RandIndex]);
        
    }   /* end of looping through all monsters */
}

void DoAttack (tPlayer * p, istream & sArgs)
{
    // get monster name from args
    string monname (istreambuf_iterator<char>(sArgs), {});
    monname[0] = toupper(monname[0]);
    NoMore (p, sArgs);
    bool here = false;

    if (monname.empty())
        *p << "Attack what?\r\n";

    for (tPlayerListIterator listiter1 = playerlist.begin ();
     listiter1 != playerlist.end ();
     listiter1++)
    {
        tPlayer *inRoom = *listiter1;
        if (inRoom->room == p->room)  // check if player is in the same room
        {
            if (inRoom->playername == monname && !here)
            {   
                here = true;
                //int dam = rand() % (9 - 7 + 1) + 7; // calculate damage value; base damage is 7-9
                int dam = 7;
                tPlayer *pTarget = inRoom;
                inRoom = 0;
                pTarget->health = pTarget->health - dam;
                *p << "You throw a punch at " << pTarget->playername << ", dealing " << dam << " damage.\r\n";
                *pTarget << p->playername << " throws a punch at you, dealing " << dam << " damage!\r\n";
                //*p << monname << " has " << mTarget->monhealth << " health remaining.\r\n";
            }
        }
    }
    for (monsterListIterator listiter1 = monmap.begin ();
     listiter1 != monmap.end ();
     listiter1++)
    {
        monster *inRoom = *listiter1;
        if (inRoom->room == p->room)  // check if monster is in the same room
        {
            if (inRoom->name == monname && !here)
            {   
                here = true;
                //int dam = rand() % (9 - 7 + 1) + 7; // calculate damage value; base damage is 7-9
                int dam = 7;
                monster *mTarget = inRoom;
                inRoom = 0;
                mTarget->monhealth = mTarget->monhealth - dam;
                *p << "You throw a punch at " << mTarget->name << ", dealing " << dam << " damage.\r\n";
                mTarget->immobile = true;
                mTarget->behavior = "hostile";
				mTarget->aggressor = (int*)p;
                //*p << monname << " has " << mTarget->monhealth << " health remaining.\r\n";
            }
        }
    }   /* end of looping through all monsters */
}
void DoScore(tPlayer * p, istream & sArgs) 
{
	SendToAll(p->playername+"'s score is "+to_string(p->score)+"\n");
}
void DoHealthCheck (tPlayer * p, istream & sArgs)
{
    NoMore (p, sArgs);
    *p << "Your current health is " << p->health << "/" << BASE_HEALTH << ".\r\n";
}

void DoBoard(tPlayer * p, istream & sArgs) 
{
	map<string, int> condition;
	condition["post"] = 1;
	condition["leaf"] = 2;
	condition["read"] = 3;
	condition["remv"] = 4;
	string boardName;
	string command;
	string result;
	int arg1 = 0;
	int arg2 = 0;
	board * targetBoard;
	sArgs >> ws >> boardName >> ws >> command;
	if (command == "read" || command == "leaf") {
		sArgs >> ws >> arg1;
		if (command == "leaf") {
			sArgs >> ws >> arg2;
		}
	}
	//command = GetMessage(sArgs, "usage is board name command (post, leaf, read, remv");
	NoMore(p, sArgs);
    for (boardListIterator listiter1 = bmap.begin ();
     listiter1 != bmap.end ();
     listiter1++)
    {
		board *b = *listiter1;
		if ((*b).bname == boardName) 
		{
			targetBoard = b;
		}
	}
	switch(condition[command]) 
	{
		case 1: 
		{
			(*targetBoard).user = (int*)&p;
			result = (*targetBoard).post(sArgs);
			*p << result;
			break;
		}
		case 2:
		{
			(*targetBoard).user = (int*)&p;
			targetBoard->load();
			result = (*targetBoard).leaf(arg1, arg2);			
			(*targetBoard).user = NULL;
			*p << "Board has:\r\n" << result;
			break;
		}
		case 3:
		{
			(*targetBoard).user = (int*)&p;
			targetBoard->load();
			result = (*targetBoard).boardread(arg1);
			(*targetBoard).user = NULL;
			*p << "Note reads:\r\n" << result;
			break;
		}
		case 4:
		{
			break;
		}
	}
}


void ProcessCommand (tPlayer * p, istream & sArgs)
{

  string command;
  sArgs >> command >> ws;   // get command, eat whitespace after it
  
  // first see if they have entered a movement command (eg. n, s, e, w)
  set<string>::const_iterator direction_iter = directionset.find (command);
  if (direction_iter != directionset.end ())
    DoDirection (p, command);
  else
    {
    // otherwise, look up command in commands map  
    map<string, tHandler>::const_iterator command_iter = commandmap.find (command);
    if (command_iter == commandmap.end ())
       throw runtime_error ("Huh?");      // don't get it
 
    command_iter->second (p, sArgs);  // execute command (eg. DoLook)
    }
    //DoMonsterMove ();
} /* end of ProcessCommand */

void LoadCommands ()
  {
  commandmap ["look"]     = DoLook;        // look around
  commandmap ["l"]        = DoLook;        // synonymm for look
  commandmap ["quit"]     = DoQuit;        // bye bye
  commandmap ["say"]      = DoSay;         // say something
  commandmap ["\""]       = DoSay;         // synonym for say
  commandmap ["tell"]     = DoTell;        // tell someone
  commandmap ["shutdown"] = DoShutdown;    // shut MUD down
  commandmap ["help"]     = DoHelp;        // show help message
  commandmap ["goto"]     = DoGoTo;        // go to room
  commandmap ["transfer"] = DoTransfer;    // transfer someone else
  commandmap ["setflag"]  = DoSetFlag;     // set a player's flag
  commandmap ["clearflag"]= DoClearFlag;   // clear a player's flag
  commandmap ["save"]     = DoSave;        // save a player
  commandmap ["chat"]     = DoChat;        // chat
  commandmap ["emote"]    = DoEmote;       // emote
  commandmap ["who"]      = DoWho;         // who is on?
  commandmap ["kick"]     = DoKick;        // screw that guy
  commandmap ["wait"]     = DoWait;        // wait for a while
  commandmap ["."]        = DoWait;        // synonym for wait
  commandmap ["attack"]   = DoAttack;      // attack an entity
  commandmap ["kill"]	  = DoAttack;	   // synonym for attack
  commandmap ["health"]   = DoHealthCheck; // show the player's current health value
  commandmap ["h"]        = DoHealthCheck; // synonym for health 
  commandmap["?"]		  = DoScore; 
  commandmap["board"]	  = DoBoard;  
} // end of LoadCommands

