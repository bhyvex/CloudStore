#include "NameSpace.h"


NameSpace::NameSpace()
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



