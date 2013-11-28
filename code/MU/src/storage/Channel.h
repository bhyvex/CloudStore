#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include "NameSpace.h"

class Channel
{
public:

	Channel();
	~Channel();

	int m_Id;
	NameSpace *m_DataNS;
	NameSpace *m_MetaNS;
};



#endif
