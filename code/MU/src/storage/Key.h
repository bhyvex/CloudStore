#ifndef _KEY_H_
#define _KEY_H_

#include <iostream>
using namespace std;

struct MUKeyInfo
{
	string userID;
	string PID;
	string FileName;
};

class Key
{
public:

	static string serialize(MUKeyInfo *keyinfo);
	static MUKeyInfo deserialize(string key);
};

#endif
