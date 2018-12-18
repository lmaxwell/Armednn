#include "Eigen/Dense"
#include <iostream>

#ifndef _CORE_H
#define _CORE_H

namespace Tsnn{


//Row Major Matrix
typedef Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Matrix; 
//Row Vector
typedef Eigen::Matrix<float,1,Eigen::Dynamic> Vector; 


/* simple logging*/
enum typelog {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

struct nullstream: std::ostream{
    nullstream(): std::ostream(0){}
};

/*
template <typename T>
    nullstream &operator<<(nullstream &o, T const & x) { return o;}
*/


class LogMessage{
    std::string level;
    std::ostream &ofs;
    static bool _enable;
    nullstream  __nullstream;
    public:
    LogMessage(const std::string &l)
        :level(l), ofs(std::cerr) {
            if(_enable)
            {
                stream() << "[" << level << "]\t";
            }
        }
    LogMessage(const std::string &l,const char* file ,const char*func, int line)
        :level(l), ofs(std::cerr) {
            if(_enable)
            {
                stream() << file<<"("<<line<<")" <<": "<<func<<" [" << level << "]\t";
            }
        }
    LogMessage(std::ostream &o)
        :level("ERROR"), ofs(o) {
            stream() << "[" << level << "]\t";
        }
    inline std::ostream &stream() {
            return  _enable? ofs : __nullstream;
    }
    ~LogMessage() {
        stream() << std::endl;
    }


    //static bool is_enabled();

    static void enable(bool __enable) {
        _enable = __enable;
    }
};

#define LOG(type)        LogMessage(#type,__FILE__,__func__,__LINE__).stream()

#define REGIST            LOG(REGIST) 

#define INFO             LOG(INFO) 
#define DEBUG            LOG(DEBUG) 

#define CHECK(x)         if (x) {} else  LOG(ERROR) <<"\""<< #x<<"\"\t" 
#define DCHECK(x)        CHECK(x)

#define CHECK_EQ(x, y)   CHECK((x) == (y))
#define CHECK_LT(x, y)   CHECK((x) < (y))
#define CHECK_GT(x, y)   CHECK((x) > (y))
#define CHECK_LE(x, y)   CHECK((x) <= (y))
#define CHECK_GE(x, y)   CHECK((x) >= (y))
#define CHECK_NE(x, y)   CHECK((x) != (y))
#define CHECK_NOTNULL(x) \
        ({ \
                 CHECK_NE(x, NULL); \
                 (x); \
             })
#define DCHECK_EQ(x, y)  DCHECK((x) == (y))
#define DCHECK_LT(x, y)  DCHECK((x) < (y))
#define DCHECK_GT(x, y)  DCHECK((x) > (y))
#define DCHECK_LE(x, y)  DCHECK((x) <= (y))
#define DCHECK_GE(x, y)  DCHECK((x) >= (y))
#define DCHECK_NE(x, y)  DCHECK((x) != (y))


}

#endif

