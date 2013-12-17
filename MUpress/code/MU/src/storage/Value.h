#ifndef _VALUE_H_
#define _VALUE_H_

#include <iostream>
using namespace std;



struct MUValueInfo
{
	string fid;
};

class Value
{
public:

	static string serialize(MUValueInfo *valueinfo);
	static MUValueInfo deserialize(string value);
};

#endif

