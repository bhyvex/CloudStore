#include "ChannelManager.h"
#include "Channel.h"
#include "frame/MUConfiguration.h"
#include "HashMappingStrategy.h"



ChannelManager::ChannelManager()
{
}

ChannelManager::~ChannelManager()
{
}



bool ChannelManager::init()
{
	m_ChannelVec.resize(MUConfiguration::getInstance()->m_ChannelNum);

	return true;
}

bool ChannelManager::createChannel(int ChannelID, string RootPath)
{
	Channel* pChannel = NULL;

	if(ChannelID > m_ChannelVec.size()){
		return false;

	//Data Channel
	}else if (ChannelID != MUConfiguration::getInstance()->m_MainChannelID){
		pChannel = new Channel();
		pChannel->setID(ChannelID);
		pChannel->setRoot(RootPath);
		pChannel->setDataNS(KV);
		pChannel->setSizeInMB();
		
		m_ChannelVec[ChannelID] = pChannel;
		//m_ChannelVec.shrink_to_fit(); //need C++11
		return true;

	//Main Chennel
	}else if (ChannelID == MUConfiguration::getInstance()->m_MainChannelID){
		pChannel = new Channel();
		pChannel->setID(ChannelID);
		pChannel->setRoot(RootPath);
		pChannel->setDataNS(FS);
		pChannel->setSizeInMB();
		
		m_ChannelVec[ChannelID] = pChannel;
		//m_ChannelVec.shrink_to_fit(); //need C++11
		return true;
	}
}

bool ChannelManager::deleteChannel(int ChannelID)
{
	if(ChannelID > m_ChannelVec.size()){
		return false;
	}else{
		Channel* pChannel = m_ChannelVec[ChannelID];
		m_ChannelVec.erase(m_ChannelVec.begin() + ChannelID);
		//m_ChannelVec.shrink_to_fit(); //need C++11

		delete pChannel;

		return true;
	}
}

Channel* ChannelManager::findChannel(int ChannelID)
{
	if(ChannelID > m_ChannelVec.size()){
		return NULL;
	}else{
		return m_ChannelVec[ChannelID];
	}
}


int ChannelManager::ChannelSize()
{
	return m_ChannelVec.size();
}



bool ChannelManager::createStrategy(enum MappingStrategy Strategy)
{
	if(Strategy == SimpleHash){
		m_ChannelMappingStrategy = new HashMappingStrategy();
	}else{//default
		m_ChannelMappingStrategy = new HashMappingStrategy();
	}

	return true;
}



Channel* ChannelManager::Mapping(uint64_t BucketID)
{
	uint64_t ChannelID = m_ChannelMappingStrategy->Mapping(BucketID);
	
	return findChannel(ChannelID);
}


