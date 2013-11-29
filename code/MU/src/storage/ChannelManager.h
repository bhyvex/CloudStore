#ifndef _CHANNELMANAGER_H_
#define _CHANNELMANAGER_H_

#include <vector>
#include <string>
#include <inttypes.h>
#include <math.h>

#include "sys/sys.h"


using namespace std;

class Channel;
class ChannelMappingStrategy;

enum MappingStrategy
{
	SimpleHash
};

class ChannelManager : public Singleton<ChannelManager>
{
	friend class Singleton<ChannelManager>;
	
public:

	bool init();

	
	bool createChannel(int ChannelID, string RootPath);
	bool deleteChannel(int ChannelID);
	Channel* findChannel(int ChannelID);
	int ChannelSize();
	vector<Channel*> m_ChannelVec;


	bool createStrategy(enum MappingStrategy Strategy);
	Channel* Mapping(uint64_t BucketID);
	ChannelMappingStrategy *m_ChannelMappingStrategy;

private:
	ChannelManager();
	~ChannelManager();
	ChannelManager &operator=(const ChannelManager &conf);

	
};


#endif
