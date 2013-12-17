#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "NameSpace.h"
#include <inttypes.h>


class Channel
{
public:

	Channel();
	~Channel();

	void setID(int ID);
	bool setRoot(string path);
	bool setDataNS(enum NameSpaceType);
	bool setSizeInMB();

	int m_Id;
	string m_Root;
	int m_SizeInMB;
	
	NameSpace *m_DataNS;
	NameSpace *m_MetaNS;
};



#endif
