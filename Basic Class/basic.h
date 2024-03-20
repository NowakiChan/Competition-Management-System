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
/*data info code*/
#define NORMAL_DATA 1000
#define WRONG_FORMAT 1001
#define DUPLICATE_DATA 1002
#define SIMILAR_DATA 1003 
/*basic define of user info*/
#define NAME "name"
#define PWD "password"
#define PHNUMBER "phone_number"
#define ADDRESS "mail_address"
/*define of inherited user group*/
#define APARTMENT "apartment"
#define COLLEGE "college"
#define PROFESSION "prefession"
#define LEVEL "level"
#define AMOUNT "work_amount"
#define GRADE "grade"
/*user type*/
#include<iostream>
#include<string>
#include<time.h>
#include<ctype.h>
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
    if(id.size() >= 2 && isdigit(id[0]) && isdigit(id[1])){
        std::string res("20");
        return res + id.substr(0,2);
    }
    
    return std::string("NULL");
}

#endif