#ifndef TINYMUDSERVER_ROOM_H
#define TINYMUDSERVER_ROOM_H

#include <map>

// map of exits for rooms
typedef std::map<std::string, int> tExitMap;

// a room (vnum of room is in the room map)
class tRoom   
  {
  public:
  
  std::string description;   // what it looks like
  tExitMap exits;       // map of exits

  // ctor
  tRoom (const std::string & s) : description (s) {}
  };  // end of class tRoom

// we will use a map of rooms
typedef std::map <int, tRoom*> tRoomMap;
typedef tRoomMap::const_iterator tRoomMapIterator;

tRoom * FindRoom (const int & vnum);
  
#endif // TINYMUDSERVER_ROOM_H

