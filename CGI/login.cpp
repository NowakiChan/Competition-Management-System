#include"../Function Class/login.h"
#include<iostream>

#define ENV_NAME "CONTENT_LENGTH"
std::string getRequest()
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

int main()
{
    Login::plugin().initiate( getRequest() );

    std::cout<<"Content-type: application/json\n\n";
    std::cout<<Login::plugin().getResult();

    return 0;
}