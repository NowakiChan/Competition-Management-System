#include"../Function Class/login.h"

int main()
{

    std::cout<<"Content-type: application/json\n\n";
    
    const int status_code = Register::singleton().setData( postRequest() );
    if(status_code == OK){
        std::cout<<Register::singleton().createAccount();
    }
    else if(status_code == FAIL) std::cout<<Register::singleton().returnFail();
    else if(status_code == ERROR) std::cout<<Register::singleton().returnError();
    else if(status_code == DUPLICATED) std::cout<<Register::singleton().returnDuplicated();
    return 0;
}