#ifndef _CHANNELMAPPINGSTRATEGY_H_
#define _CHANNELMAPPINGSTRATEGY_H_

#include <inttypes.h>



class ChannelMappingStrategy
{
public:
	ChannelMappingStrategy();
	~ChannelMappingStrategy();

	virtual uint64_t Mapping(int BucketID) = 0;

};


#endif
