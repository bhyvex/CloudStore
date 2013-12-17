#ifndef _HASHMAPPINGSTRATEGY_H_
#define _HASHMAPPINGSTRATEGY_H_


#include <iostream>
#include <vector>
#include "ChannelMappingStrategy.h"
#include "storage/ChannelManager.h"

using namespace std;

struct ChannelTable
{
	vector<int> BucketID;
	int State;
};

struct BucketTable
{
	vector<int> UserID;
	int ChannelID;
	int State;
};

struct UserTable
{
	int BucketID;
	int State;
};


class HashMappingStrategy : public ChannelMappingStrategy
{
public:
	HashMappingStrategy();
	~HashMappingStrategy();

	virtual uint64_t Mapping(int BucketID);//must be quick
	
	bool InsertChannelTable();
	bool InsertBucketTable();
	bool InsertUserTable();
	
	bool DeleteChannelTable();
	bool DeleteBucketTable();
	bool DeleteUserTable();
	
	bool UpdateChannelTable();
	bool UpdateBucketTable();
	bool UpdateUserTable();
	

	vector<ChannelTable> m_ChannelTable;
	vector<BucketTable> m_BucketTable;
	vector<UserTable> m_UserTable;
};


#endif
