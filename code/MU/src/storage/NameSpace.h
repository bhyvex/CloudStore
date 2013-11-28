#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_

class NameSpace 
{
public:
	NameSpace();
	~NameSpace();

	//interface like posix
	virtual int Open() = 0;
	virtual int Close() = 0;
	virtual int Stat() = 0;
	virtual int Move() = 0;
};


#endif
