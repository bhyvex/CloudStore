/*
 * =====================================================================================
 *
 *       Filename:  Tool.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2010年03月19日 14时23分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), duanhancong@uestc.edu.cn
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */
#include "Tool.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include <cmath>
#include "BaseHeader.h"
#include <sys/signal.h>
#include "Error.h"
//#include "Exception.h"
using namespace std;

unsigned int Tool::strToInt(const string& sValue)
{
    if (sValue == "")
        return 0;
    stringstream sst;
    unsigned int iValue;
    sst << sValue;
    sst >> iValue;
    return iValue;

}
std::string Tool::intToStr(unsigned int iValue)
{
    stringstream sst;
    std::string sValue;
    sst << iValue;
    sst >> sValue;
    return sValue;

}
unsigned long long
Tool::strToLong(const string& sValue)
{
    stringstream sst;
    unsigned long long iValue;
    sst << sValue;
    sst >> iValue;
    return iValue;

}
std::string
Tool::longToStr(unsigned long long iValue)
{
    stringstream sst;
    string sValue;
    sst << iValue;
    sst >> sValue;
    return sValue;
}
float Tool::strToFloat(const string& sValue)
{
    stringstream sst;
    float iValue;
    sst << sValue;
    sst >> iValue;
    return iValue;
}
std::string Tool::floatToStr(float fValue)
{
    stringstream sst;
    std::string sValue;
    sst << fValue;
    sst >> sValue;
    return sValue;
}
std::string Tool::getTableName(int cmd)
{
    /* string tbName;
    ime_t tTemp = time(NULL);
    struct tm *tmTemp = localtime(&tTemp);
    string year = intToStr(tmTemp->tm_year + 1900);
    string mon = intToStr(tmTemp->tm_mon + 1);
    switch(cmd)
    {
    	case MSG_CU_LS_DATACHECK_REQ://
               tbName="CUCheckAcquire"+year+mon;
    			   break;
    	case MSG_CS_LS_DATACHECK_REQ:
               tbName="CSCheckAcquire"+year+mon;
    		   break;
    //	case WRITE_LOG_FOR_MASTER:
    	case MSG_MASTER_LS_WRITE_LOG:
               tbName="masterLog"+year+mon;
    		   break;
    	case MSG_CU_LS_WRITE_LOG:
               tbName="cuLog"+year+mon;
    		   break;
    	case MSG_CS_LS_WRITE_LOG:
               tbName="csLog"+year+mon;
    		   break;
    	case MSG_TS_LS_WRITE_LOG:
               tbName="tsLog"+year+mon;
    		   break;
    	case MSG_PORTAL_LS_WRITE_LOG:
    		   tbName="wufaqueding";
    	default:
    		   tbName="";
    		   break;

    }
    return tbName;*/
    return "";
}
int Tool::parseData(char *data,std::vector<std::string> &vecInfo,char spc)
{
    char *chrTmp = data;
    string strTmp(data);
    int pos=0;
    char *idx;
    idx = strchr(chrTmp,spc);
    // in case the first char is |
    if (idx == chrTmp)
    {
        ++pos;
        idx = strchr(chrTmp+pos,spc);
    }
    vecInfo.push_back(strTmp.substr(pos,idx-chrTmp));
    pos+=idx-chrTmp;
    ++pos;
    char *idx2;
    while (*idx == '|')
    {
        idx2 = ++idx;
        if ((idx2=strchr(idx2,spc))==NULL)
        {
            if (strTmp.substr(pos).length()==0)
                break;
            vecInfo.push_back(strTmp.substr(pos));
            break;
        }
        vecInfo.push_back(strTmp.substr(pos,idx2-idx));
        pos = pos + (idx2 - idx) + 1;
        idx=idx2;
    }
    return 0;
}

string Tool::replacePath(string oldPath,const string &newPath)
{
    if (!oldPath.length())
        return "";
    char sep='/';
#ifdef _WIN32
    sep='\\';
#endif
    string::size_type i=oldPath.rfind(sep,oldPath.length());
    if (i != string::npos)
    {
        return oldPath.replace(0,(oldPath.substr(0,i)).length(),newPath);
    }
    else
    {
        return (oldPath=(newPath+"/"+oldPath));
    }
}

int
Tool::setThreadMask(void)
{
    sigset_t Snew;

    if (sigemptyset(&Snew) ||
            sigaddset(&Snew, SIGPIPE) ||
            sigaddset(&Snew, SIGCHLD) ||
#ifdef _SQUID_LINUX_THREADS_
            sigaddset(&Snew, SIGQUIT) ||
            sigaddset(&Snew, SIGTRAP) ||
#else
            sigaddset(&Snew, SIGUSR1) ||
            sigaddset(&Snew, SIGUSR2) ||
#endif
            sigaddset(&Snew, SIGHUP) ||
            sigaddset(&Snew, SIGTERM) ||
            sigaddset(&Snew, SIGINT) ||
            sigaddset(&Snew, SIGALRM))
    {
        return FAILED;
    }
    if (pthread_sigmask(SIG_BLOCK, &Snew, NULL))
    {
        return FAILED;
    }
    return SUCCESSFUL;
}

int
Tool::getCurrentDateStr(string &dateStr, time_t tTemp)
{
    if(-1 == tTemp)
    {
        tTemp = time(NULL);
    }
    tm *tmTemp = new tm();
    tmTemp = localtime_r(&tTemp, tmTemp);
    char buf[9];
    if (0 == strftime(buf, 9, "%Y%m%d", tmTemp))
    {
        return FAILED;
    }
    dateStr.assign(buf);
    delete tmTemp;
    return SUCCESSFUL;
}

int
Tool :: getCurrentTimeStr(string &timeStr, time_t tTemp)
{
    if(-1 == tTemp)
    {
        tTemp = time(NULL);
    }
    tm *tmTemp = new tm();
    tmTemp = localtime_r(&tTemp, tmTemp);
    char buf[21];
    if (0 == strftime(buf, 21, "%Y-%m-%d %H:%M:%S", tmTemp))
    {
        return FAILED;
    }
    timeStr.assign(buf);
    delete tmTemp;
    return SUCCESSFUL;
}

//convert time string formatted as "1970-01-01 00:00:00" into corresponding timestamp
time_t
Tool::getTimeStamp(const string &date)
{
    int year, month, day, hour, min, sec;
    const int inputNum = 6;
    if (sscanf(date.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec) != inputNum)
    {
        return FAILED;
    }
    tm mytm;
    mytm.tm_year = year - 1900;
    mytm.tm_mon = month -1;
    mytm.tm_mday = day;
    mytm.tm_hour = hour;
    mytm.tm_min = min;
    mytm.tm_sec = sec;

    return mktime(&mytm);
}

int
Tool :: calculateChkSum(const string &value)
{
    size_t flag = value.size() / 2, size = value.size();
    int sum = 0, factor = 100000;

    for (size_t i = 0; i < flag; ++i)
    {
        sum += value[i] * value[size - i - 1];
        sum %= factor;
    }
    return sum;

    //const unsigned int factor = 10000000;
    //int chkSum = 0;
    //unsigned int num = 0;
    //int len = value.length();

    //for (int i = 0; i < len; ++i)
    //{
    //    num = value[len - i - 1] << (i + 1);
    //    chkSum += num;
    //    chkSum %= factor;
    //}
    //return chkSum;
}

int
Tool :: replaceDU(string &srcDU)
{
    //SD_RNC_BSC6810V200R010ENGC01B050_0_4_111123 => SD_234234_0_4_111123
    // RD_TOP_BSC6810V200R010ENGC01B050_111124 => RD_453452_111124
    size_t pos1, pos2, pos3;
    string objSrc;

    if (string::npos == (pos1 = srcDU.find('_')) ||
        string::npos == (pos2 = srcDU.find('_', pos1 + 1)) ||
        string::npos == (pos3 = srcDU.find('_', pos2 + 1)))
    {
        return FAILED;
    }
    objSrc.assign(srcDU.substr(pos1 + 1, pos3 - pos1 - 1));
    int chksum = calculateChkSum(objSrc);
    srcDU.replace(pos1 + 1, pos3 - pos1 - 1, convertType<int, string>(chksum));
    return SUCCESSFUL;

}

int
Tool :: getTimeValue(const string &date)
{
    string time;
    if (date.size() < 6)
    {
        return FAILED;
    }
    if (6 == date.size())
    {
        time = "20" + date;
    }
    else
    {
        time.assign(date);
    }
    //cout << "getTimeValue  " << time.substr(0, 4) << " " << time.substr(4, 2) << " " << time.substr(6, 2) << endl;
    time = time.substr(0, 4) + "-"
         + time.substr(4, 2) + "-"
         + time.substr(6, 2)
         + " 00:00:00";
    time_t timestamp = getTimeStamp(time);
    return timestamp / (3600 * 24);
}

//void
//Tool :: getXMLValue(XMLConfig &xml, const char* path, string &value)
//{
    //if (FAILED == xml.getFirstNodeValue(path, value))
    //{
        //string str = "getFirstNodeValue(" + string(path) + ") ERROR!";
        //THROW_EX(str);
    //}
    ////if ("" == value)
    ////{
    ////    string str = string(path) + " values is empty";
    ////    THROW_EX(str);
    ////}
//}

//void
//Tool :: getXMLNodeValue(XMLConfig &xml, XMLNode *pNode, string &value)
//{
    //if (FAILED == xml.getValue(pNode, value))
    //{
        //string str = "getValue ERROR!";
        //THROW_EX(str);
    //}
    ////if ("" == value)
    ////{
    ////    string str = " values is empty";
    ////    THROW_EX(str);
    ////}
//}

//XMLNode*
//Tool :: addSimpleXMLNode(XMLConfig &xml, XMLNode *pRoot,
        //const char* name, const char* value)
//{
    //XMLNode *pNode = NULL;
    //if (NULL == (pNode = xml.addSimpleChild(pRoot, name, value)))
    //{
        //THROW_EX("addSimpleChild error");
    //}
    //return pNode;
//}

//XMLNode*
//Tool :: addXMLNode(XMLConfig &xml, XMLNode *pRoot, const char* name)
//{
    //XMLNode *pNode = NULL;
    //if (NULL == (pNode = xml.addChild(pRoot, name)))
    //{
        //THROW_EX("addSimpleChild error");
    //}
    //return pNode;
//}

//void
//Tool :: getXMLProperty(XMLConfig &xml, XMLNode *pNode, const char* propName, string &value)
//{
    //if (FAILED == xml.getProperty(pNode, propName, value))
    //{
        //string str = "getXMLProperty(" + string(propName) + ") ERROR!";
        //THROW_EX(str);
    //}
    ////if ("" == value)
    ////{
    ////    string str = string(propName) + " values is empty";
    ////    THROW_EX(str);
    ////}
//}


//void
//Tool :: setXMLProperty(XMLConfig &xml, XMLNode *pNode,
            //const char* name, const char* value)
//{
    //if (FAILED == xml.setProperty(pNode, name, value))
    //{
        //THROW_EX("setProperty error");
    //}
//}

