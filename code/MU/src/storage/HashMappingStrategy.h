#ifndef _HASHMAPPINGSTRATEGY_H_
#define _HASHMAPPINGSTRATEGY_H_


#include <iostream>
#include <vector>
#include "ChannelMappingStrategy.h"

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

	ChannelBucket Mapping(int UserID);//must be quick
	
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
