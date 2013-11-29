#include "Channel.h"

Channel::Channel()
{
}

Channel::~Channel()
{

}

void Channel::setID(int ID)
{
	m_Id = ID;
}

bool Channel::setRoot(string path)
{
	if(1){
		m_Root = path;
		return true;
	}else{
		return false;
	}
}


