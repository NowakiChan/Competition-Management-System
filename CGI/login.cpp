#include"../Function Class/login.h"
#include<iostream>

int main()
{
    Login::plugin().initiate( getRequest() );

    std::cout<<"Content-type: application/json\n\n";
    std::cout<<Login::plugin().getResult();

    return 0;
}