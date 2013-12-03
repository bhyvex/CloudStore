#include "HashMappingStrategy.h"

HashMappingStrategy::HashMappingStrategy()
{
}

HashMappingStrategy::~HashMappingStrategy()
{
}

uint64_t HashMappingStrategy::Mapping(int BucketID)
{
	//sub the MainChannel
	return BucketID % (ChannelManager::getInstance()->ChannelSize() - 1);
}


bool HashMappingStrategy::InsertChannelTable()
{
	return true;
}

bool HashMappingStrategy::InsertBucketTable()
{
	return true;
}


bool HashMappingStrategy::InsertUserTable()
{
	return true;
}


bool HashMappingStrategy::DeleteChannelTable()
{
	return true;
}


bool HashMappingStrategy::DeleteBucketTable()
{
	return true;
}


bool HashMappingStrategy::DeleteUserTable()
{
	return true;
}


bool HashMappingStrategy::UpdateChannelTable()
{
	return true;
}


bool HashMappingStrategy::UpdateBucketTable()
{
	return true;
}


bool HashMappingStrategy::UpdateUserTable()
{
	return true;
}


