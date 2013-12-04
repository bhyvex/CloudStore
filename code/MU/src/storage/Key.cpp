#include "Key.h"

#define KEY_SEPARATOR "/"

string Key::serialize(MUKeyInfo *keyinfo)
{
	string key = keyinfo->userID + 
					KEY_SEPARATOR +
					keyinfo->PID +
					KEY_SEPARATOR +
					keyinfo->FileName;
	return key;
}

MUKeyInfo Key::deserialize(string key)
{
	size_t pos;
	MUKeyInfo keyinfo;

	pos = key.find(KEY_SEPARATOR);
	keyinfo.userID = key.substr(0, pos);
	key = key.substr(pos+1);

	pos = key.find(KEY_SEPARATOR);
	keyinfo.PID = key.substr(0, pos);
	key = key.substr(pos+1);

	keyinfo.FileName = key;
	
	
	return keyinfo;
}

