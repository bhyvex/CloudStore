/*
 * =====================================================================================
 *
 *       Filename:  Lock.h
 *
 *    Description:  RAII(Resource Acquisition Is Initialization)实现模板
 *
 *        Version:  1.0
 *        Created:  04/25/2011 09:19:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef COMMON_SYS_LOCK_H_
#define COMMON_SYS_LOCK_H_

#include "Mutex.h"

/**
 * @brief 对临界资源的Guard，在构造器中调用Mutex的lock方法，
 * 在析构函数调用unlock方法，通过实例化Lock类型的局部
 * 变量，则在此变量作用域内持有锁
 */
class Lock {
public:
    Lock(const Mutex &mutex) :
        m_Mutex(mutex)
    {
        m_Mutex.lock();
    }

    ~Lock()
    {
        m_Mutex.unlock();
    }
    
private:
    // can't copy
    Lock(const Lock&);
    Lock& operator=(const Lock&);

private:
    const Mutex &m_Mutex;
};

// 防止误写为Lock(mutex)，即少写了Lock类型的变量名，
// 导致对象生成以后马上析构，锁也随即释放了
// 由于protocol buffer内部实现中有一个Lock()函数，此宏定义将使编译出错，
// 因此注释掉，程序中使用Lock时一定要注意声明变量
//#define Lock(x) static_assert(false, "missing lock variable name")
//const bool MISSING_LOCK_VARIABLE_NAME = false;
//#define Lock(x) assert(MISSING_LOCK_VARIABLE_NAME);

#endif  // COMMON_SYS_LOCK_H_

