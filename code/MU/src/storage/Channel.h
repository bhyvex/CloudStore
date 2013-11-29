#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "NameSpace.h"

class Channel
{
public:

	Channel();
	~Channel();

	void setID(int ID);
	bool setRoot(string path);

	int m_Id;
	string m_Root;
	
	NameSpace *m_DataNS;
	NameSpace *m_MetaNS;
};



#endif
