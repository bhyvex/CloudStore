#ifndef _CHANNELMAPPINGSTRATEGY_H_
#define _CHANNELMAPPINGSTRATEGY_H_


struct ChannelBucket
{
	int ChannelID;
	int BucketID;
};


class ChannelMappingStrategy
{
public:
	ChannelMappingStrategy();
	~ChannelMappingStrategy();

	virtual ChannelBucket Mapping(int UserID) = 0;

};


#endif
