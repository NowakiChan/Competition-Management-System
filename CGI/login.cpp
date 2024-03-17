#include"../Function Class/login.h"
#include<iostream>

std::string getRequest()
{
    char* request_len_str = getenv("MESSGAE_LEN");
    const int request_len = atoi(request_len_str);
    std::string str;
    for(int i = 0;i < request_len;i++)
        str += (char)getchar();
    
    return str;
}

int main()
{
    login::plugin().initiate( getRequest() );

    std::cout<<"application/json";
    std::cout<<login::plugin().getResult();

    return 0;
}