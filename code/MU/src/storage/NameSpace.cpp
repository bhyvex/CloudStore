#include "NameSpace.h"


NameSpace::NameSpace(string path):
	m_Root(path)
{
}


NameSpace::~NameSpace()
{
}


bool NameSpace::setRoot(string path)
{
	if(1){
		m_Root= path;
		return true;
	}else{
		return false;
	}
}



