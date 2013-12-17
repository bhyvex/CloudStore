使用方法：
(1)定义一个全局变量g_pDevLog(不能改其它的)
DevLog *g_pDevLog = NULL;

(2)使用三个宏来写开发日志
DEV_LOG(LOG_LEVEL level, LOG_OUT_TYPE outtype, string msg);
记录日志的通用宏，level是级别，outtype是输出类型, msg是错误信息
LOG_OUT_TYPE表示日志的输出类型，有三种：OUT_SCR只输出到屏幕，OUT_FILE只输出到文件，OUT_BOTH屏幕打印并且输出到文件

DEV_LOG_ERROR(string msg);
在出错时用该宏记录日志，等价于DEV_LOG(LERROR, OUT_BOTH, msg);

DEV_LOG_FUNC();
在重要的函数入口处添加，无参数。


========================================================================================
接口说明：

1.构造函数
DevLog();//无参数，自动生成日志文件名，文件名格式为“log_时间”，例如：log_213548
使用该构造函数时，每次生成的文件名不相同

DevLog(const char* logName);//参数为日志文件名
使用该构造函数，每次都写入指定的文件中，如果不存在则自动创建，如果已经存在则在文件尾部append

2.设置日志级别
int setLogLevel(LOG_LEVEL level);
level的取值包括：LEVEL_NOE, LEVEL_LOW, LEVEL_MIDDLE, LEVEL_HIGH


3.写日志
int writeLog(LOG_LEVEL type, LOG_OUT_TYPE outtype, const std::string& message);
type表示日志的类型，取值如下：
LINFO=0,  //一般信息，主要是各种xml，内容较长
LDEBUG,   //调试信息，如某个变量的值
LFUNC,    //进入函数，在重要的函数入口处使用
LEVENT,   //业务事件，如：收到客户端发送来的消息，开始汇总等，内容较短
LWARNING, //警告
LERROR,   //一般错误，当前任务出错，但是后续流程还能继续执行
LFATAL    //致命错误，发生该错误后，程序必须退出

LOG_OUT_TYPE表示日志的输出类型，有三种：OUT_SCR只输出到屏幕，OUT_FILE只输出到文件，OUT_BOTH屏幕打印并且输出到文件

输出到屏幕时，打印的颜色规则如下：
LINFO， //蓝色
LDEBUG, //白色
LFUNC, //白色
LEVENT, //业务事件，绿色 
LWARNING, //警告,黄色
LERROR, //一般错误，红色
LFATAL//致命错误，红色

4 添加分割行
int insertSeparatorLine(int n);
参数n表示插入n个分割行，分割行的内容默认为空行，可以通过setLineSeparator设置
int setLineSeparator(const std::string &line);


