/*
 * =====================================================================================
 *
 *       Filename:  Tool.h
 *
 *    Description:  for use
 *    *
 *        Version:  1.0
 *        Created:  2010年03月19日 14时14分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (fanbo), duanhancong@uestc.edu.cn
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */
#ifndef TOOL_H_
#define TOOL_H_
#include <cstring>
#include <cstdio>
#include <iostream>
#include <vector>
#include <sstream>
#include "BaseReq.h"

class Tool
{
public:
    static unsigned int strToInt(const std::string& sValue);
    static std::string intToStr(unsigned int iValue);
    static unsigned long long strToLong(const std::string& sValue);
    static std::string longToStr(unsigned long long iValue);
    static float strToFloat(const std::string& sValue);
    static std::string floatToStr(float fValue);
    static std::string getTableName(int cmd);
    // the parameters in the data is separeted by |
    static int parseData(char *data,std::vector<std::string> &, char spc);
    static std::string replacePath(std::string oldPath,const std::string &newPath);

    static int  setThreadMask(void);  //added by pxy 2010/07/22, for setting thread signal mask
    static int  getCurrentDateStr(std::string &dateStr,  //returns: YYYYMMDD
                time_t tTemp = -1);   //added by pxy 2010/08/23, for getting current date string
    static time_t getTimeStamp(const std::string &date);  //date: YYYY-MM-DD HH:MM:SS
    static int  getCurrentTimeStr(std::string &timeStr, time_t tTemp = -1); //returns: YYYY-MM-DD hh:mm:ss
    static int  calculateChkSum(const std::string &value);
    static int  replaceDU(std::string &srcDU);
    static int  getTimeValue(const std::string &date);
    //static void getXMLValue(XMLConfig &xml, const char* path, std::string &value);
    //static void getXMLNodeValue(XMLConfig &xml, XMLNode *pNode, std::string &value);
    //static XMLNode* addXMLNode(XMLConfig &xml, XMLNode *pRoot, const char* name);
    //static XMLNode* addSimpleXMLNode(XMLConfig &xml, XMLNode *pRoot, const char* name, const char* value);
    //static void setXMLProperty(XMLConfig &xml, XMLNode *pNode, const char* name, const char* value);
    //static void getXMLProperty(XMLConfig &xml, XMLNode *pNode, const char* propName, std::string &value);
};

template<typename S, typename D>
D convertType(const S &src)
{
    std::stringstream ss;
    D dst;

    ss << src;
    ss >> dst;
    return dst;
}

#endif
