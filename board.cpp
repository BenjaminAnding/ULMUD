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

board * FindBoard (const std::string & name)
{
    boardListIterator i =
    std::find_if (bmap.begin (), bmap.end (), findBoardName (name));
    
    if (i == bmap.end ())
        return NULL;
    else
        return *i;
} //end of FindBoard

void board::load ()
{
    // load Object file
    std::ifstream fBoard (BOARD_FILE, std::ios::in);
    if (!fBoard)
    {
        std::cerr << "Could not open objects file: " << BOARD_FILE << std::endl;
        return;
    }
    
    while (!(fBoard.eof ())) 
	{
    	std::string sMessageCode, sMessageText;
    	fBoard >> sMessageCode >> std::ws;
    	getline (fBoard, sMessageText);
    	if (!(sMessageCode.empty ()))
		{
    		contents [tolower (sMessageCode)] =
            FindAndReplace (sMessageText, "%r", "\r\n");
		}
		this->lastpost = sMessageCode;
    } // end of read loop
}

std::string board::post (std::istream & sArgs)
{
/*
  tPlayer * p = (tPlayer*)this->user;
  int s = p->GetSocket(); 
  static std::vector<char> buf (1000);  // reserve 1000 bytes for reading into
  int nRead = read (p->GetSocket(), &buf [0], buf.size ());
  if (nRead == -1)
    {
    close (s);
    if (errno != EWOULDBLOCK)
      perror ("read from player");
    return "";
    }
  if (nRead <= 0)
    {
    close (s);
    std::cerr << "Connection " << s << " closed" << std::endl;
    s = NO_SOCKET;
    return "";
    }
*/
	std::string result;
	std::string message;
/*	
  inbuf += std::string (&buf [0], nRead);
  for ( ; ; )
    {
    std::string::size_type i = inbuf.find ('\n');
    if (i == std::string::npos)
      break;  

    std::string sLine = inbuf.substr (0, i);  
    inbuf = inbuf.substr (i + 1, std::string::npos); 
*/
  	tPlayer * p = (tPlayer*)this->user;
	close(p->GetSocket());
	while (result.find("**") == std::string::npos)
	//for (int i = 0; i < 3; i++)
	{
  		static std::vector<char> buf (1000);  // reserve 1000 bytes for reading into
  		int nRead = read (p->GetSocket(), &buf [0], buf.size ());
		if (nRead == -1) 
		{
			if (errno != EINPROGRESS)
			{
				if (errno != EWOULDBLOCK) 
				{
					perror("error in read");
				}
			}
		}
		if (nRead > 0) 
		{
  			inbuf += std::string (&buf [0], nRead);
		}
		message = inbuf.substr(0,1);
		if (!message.empty()) 
		{
			result = result + "%r" + message;
		}
	}
	return result;
}

std::string board::leaf (int lower, int upper) 
{
	std::string list;
	for (int i=lower;i < std::min(upper, (int)this->contents.size()); i++) 
	{
		list += std::to_string(i);
		list += "\r\n";
	} 
	return list+"\r\n";
}

std::string board::boardread(int which)
{
	std::string note;
	note = this->contents[std::to_string(which)]; 
	return note+"\r\n";
}
