#include <fcntl.h>
#include <sys/stat.h>
#include "comm/Error.h"
#include <time.h>
#include "DevLog.h"
using namespace std;

DevLog::DevLog():
        m_Fd(-1),
        m_ErrorCount(0),
        m_LogPath(LOGPATH),
        m_LogLevel(LEVEL_HIGH),
        m_LineSeparator(SEPARATOR_LINE)
{
}

DevLog::DevLog(const char* logName):
        m_Fd(-1),
        m_ErrorCount(0),
        m_LogPath(LOGPATH),
        m_FileNamePrefix(logName),
        m_LogLevel(LEVEL_HIGH),
        m_LineSeparator(SEPARATOR_LINE)
{
}

DevLog::~DevLog()
{
    try
    {
        if (m_Fd > 0)
        {
            stringstream bufStream;
            bufStream << "total error number: " << m_ErrorCount << endl;
            writeFile(bufStream.str());
            insertSeparatorLine();
            GREEN_MSG(bufStream.str());

            close(m_Fd);

        }
    }
    catch (...)
    {
        RED_MSG("~DevLog exception");
    }
}

int DevLog::init()
{
    if(getCurrentDate(m_CurrentDate) != SUCCESSFUL)
    {
        RED_MSG("getCurrentDate error");
        return FAILED;
    }
    if (m_FileNamePrefix.size() == 0)
    {
        m_FileNamePrefix = "log";
    }

    string path = m_LogPath;
    if (checkDir(path.c_str()) != SUCCESSFUL)
    {
        RED_MSG("checkDir failed");
        return FAILED;
    }

    string logfile = m_FileNamePrefix + "_" + m_CurrentDate;

    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if ((m_Fd = open(logfile.c_str(), O_WRONLY|O_CREAT|O_APPEND, mode)) < 0)
    {
        string errPrint = "can't create log file: " + logfile;
        RED_MSG(errPrint);
        return FAILED;
    }

    string toPrint = "\n\n=======================DevLog starts==========================\n";
    writeFile(toPrint);
    //writeLog(LEVENT, OUT_BOTH, toPrint);

    return SUCCESSFUL;
}

int DevLog::checkDir(const char* dir)
{
    GREEN_MSG("check paradir");
    struct stat buf;
    if (stat(dir, &buf) == 0)
    {
        GREEN_MSG("found it");
        return SUCCESSFUL;
    }
    else
    {
        YELLOW_MSG("not found! create it");
        if (0 != mkdir(dir, O_RDWR))
        {
            string errLog("can't mkdir ");
            errLog += dir;
            RED_MSG(errLog);
            return FAILED;
        }
    }
    return SUCCESSFUL;
}

void DevLog::writeLog(LOG_TYPE type, const string &message)
{
    string buf;
    if (getCurrentTime(buf) != SUCCESSFUL)
        RED_MSG("getCurrentTime error");
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    stringstream ss;
    ss << buf << " [" << hex << pid << ":" << tid << "] ";
    ss << setw(LEVEL_STR_MAX_LEN) << setiosflags(ios::left) << LEVEL_STR[type] << "\t" << message << endl;

    buf = ss.str();
    if (LERROR == type)
    {
        ++ m_ErrorCount;
    }
    switch (type)
    {
        case LINFO:
        case LDEBUG:
        case LFUNC:
            if (LEVEL_HIGH >= m_LogLevel)
                writeFile(buf);
            break;
        case LEVENT:
            if (m_LogLevel >= LEVEL_MIDDLE)
                writeFile(buf);
            break;
        case LWARNING:
        case LERROR:
        case LFATAL:
            if (m_LogLevel >= LEVEL_LOW)
                writeFile(buf);
            break;
    }
    //return SUCCESSFUL;
}
void DevLog::writeFile(const string &buf)
{
    //m_LogStream.write(buf.c_str(), buf.size());
    //m_LogStream.flush();
    if(checkNewFile() != SUCCESSFUL)
    {
        RED_MSG("checkNewFile failed");
    }
    if (write(m_Fd, buf.c_str(), buf.size()) < 0)
    {
        string errStr("write log file failed");
        RED_MSG(errStr);
        //return FAILED;
    }
    //return SUCCESSFUL;
}
int DevLog::checkNewFile()
{
    string curDay;
    getCurrentDate(curDay);
    if(curDay != m_CurrentDate)
    {
        string newFile = m_FileNamePrefix + "_" + curDay;
        int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        close(m_Fd);
        if ((m_Fd = open(newFile.c_str(), O_WRONLY|O_CREAT|O_APPEND, mode)) < 0)
        {
            string errPrint = "can't create log file: " + newFile;
            RED_MSG(errPrint);
            return FAILED;
        }
    }
    return SUCCESSFUL;
}

void DevLog::writeLog(LOG_TYPE level, LOG_OUT_TYPE type, const string &message)
{
    switch (type)
    {
        case OUT_SCR:
            printLog(level, message);
            break;
        case OUT_FILE:
            writeLog(level, message);
            break;
        case OUT_BOTH:
            printLog(level, message);
            writeLog(level, message);
            break;
        default:
            RED_MSG("unkown LOG_TYPE: " + intToStr(level));
    }
    //return SUCCESSFUL;
}

void DevLog::printLog(LOG_TYPE level, const string &message)
{
    switch (level)
    {
        case LINFO:
            BLUE_MSG(message);
            break;
        case LDEBUG:
        case LFUNC:
            cout << message << endl;
            break;
        case LEVENT:
            GREEN_MSG(message);
            break;
        case LWARNING:
            YELLOW_MSG(message);
            break;
        case LERROR:
        case LFATAL:
            RED_MSG(message);
            break;
        default:
            RED_MSG("unknown LOG_TYPE: " + intToStr(level));
    }
    //return SUCCESSFUL;
}

int DevLog::getCurrentTime(string &curtime)
{
    time_t t = time(NULL);
    struct tm now = *localtime(&t);

    const int timeLenth = 80;
    char timebuf[timeLenth];
    if (strftime(timebuf, timeLenth, "%Y-%m-%d %H:%M:%S", &now) != 0)
    {
        curtime = string(timebuf);
    }
    else
    {
        string errInfo("no enough space to save time string");
        RED_MSG(errInfo);
        return FAILED;
    }
    return SUCCESSFUL;
}

/*根据当前时间，得到一个文件件名字，在该文件夹下面存放本次运行的所有日志文件*/
int DevLog::getLogFileName(string &fname)
{
    time_t t = time(NULL);
    struct tm now = *localtime(&t);

    const int timeLenth = 80;
    char timebuf[timeLenth];
    if (strftime(timebuf, timeLenth, "%H%M%S", &now) != 0)
    {
        fname = "log_" + string(timebuf);
    }
    else
    {
        string errInfo("no enough space to save time string");
        RED_MSG(errInfo);
        return FAILED;
    }

    return SUCCESSFUL;
}

int DevLog::getCurrentDate(string &date)
{
    time_t t = time(NULL);
    struct tm now = *localtime(&t);

    const int timeLenth = 80;
    char timebuf[timeLenth];
    if (strftime(timebuf, timeLenth, "%Y%m%d", &now) != 0)
    {
        date = string(timebuf);
    }
    else
    {
        string errInfo("no enough space to save time string");
        RED_MSG(errInfo);
        return FAILED;
    }

    return SUCCESSFUL;
}

int DevLog::setLogLevel(LOG_LEVEL level)
{
    m_LogLevel = level;
    return SUCCESSFUL;
}

int DevLog::setLogPath(const std::string &path)
{
    m_LogPath = path;
    return SUCCESSFUL;
}

int DevLog::setLineSeparator(const string &line)
{
    m_LineSeparator = line;
    return SUCCESSFUL;
}

void DevLog::insertSeparatorLine(int n)
{
    string buf;
    for (int i=0; i<n; ++i)
        buf += m_LineSeparator;
    writeFile(buf);
    //return SUCCESSFUL;
}

//int DevLog::getProcessId()
//{
//
//
//}
//
//int DevLog::getThreadId()
//{
//
//}
