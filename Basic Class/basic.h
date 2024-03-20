#ifndef BASIC
#define BASIC
/*code type*/
#define ERROR -1
#define OK 1
#define FAIL 0
#define DUPLICATED 2
/*code type part end*/
/*user type*/
#define ROOT "root"
#define ADMIN "admin"
#define NORMAL "normal"
/*user type part end*/
#define UNDEF "undefined" /*undefined macro*/
/*environment path*/
#define ENV_NAME "CONTENT_LENGTH"
/*environment path end*/

#include<iostream>
#include<string>
#include<time.h>
std::string postRequest();
std::string getImmediateTime();
std::string generateGrade(const std::string id);

inline
std::string postRequest()
{
    char* request_len_str = getenv(ENV_NAME);
    const int request_len = atoi(request_len_str);
    std::string str;
    for(int i = 0;i < request_len;i++){
        char env_ch = getchar();
        str += (env_ch == '+') ? ' ' : env_ch;
    }
    
    return str;
}

inline
std::string getImmediateTime()
{
    time_t immediate_t = time(NULL);
    return std::string( ctime(&immediate_t) );
}

inline
std::string generateGrade(const std::string id)
{
    std::string res = "20" + id[0] + id[1];
    return res;
}

#endif