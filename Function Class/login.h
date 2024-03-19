#ifndef LOGIN
#define LOGIN
#include"../Basic Class/sql.h"
#include"../Basic Class/user.h"

#define GENERATE_STR(x) "'x'"
#define STA_VALUE_NAME "status"
#define DATA_VALUE_NAME "data"
#define PWD_VALUE_NAME "password"
#define ID_VALUE_NAME "id"
class Login
{
private:
    Login() {}
    //Json::Value getUsrInfo();
    std::string returnSuccess();
    std::string returnFail();
    std::string returnError();
    //std::string getUserPwd();
    int compare();
private:
    Json::Value front_end_info;
    Json::Value res_info;
public:
    static Login& plugin();
    std::string getResult();
    void initiate(const std::string&);
    void initiate(const char*);
    ~Login() = default;
};

Login& Login::plugin()
{
    static Login singleton;
    return singleton;
}

void Login::initiate(const std::string& usr_str)
{
    Json::Reader processer;
    processer.parse(usr_str,this->front_end_info,false);

    //sql::plugin().localConnect("root","","test");
}

void Login::initiate(const char* usr_str)
{
    Json::Reader processer;
    processer.parse(usr_str,this->front_end_info,false);
}

int Login::compare()
{
    sql::plugin().localConnect("root","","test");// sql must be init in using function

    if(front_end_info.isMember(ID_VALUE_NAME) && front_end_info.isMember(PWD_VALUE_NAME)){
        std::string info_qry = "SELECT password FROM User_Info WHERE id = " + front_end_info[ID_VALUE_NAME].asString();

        sql::plugin().query(info_qry);
        if(sql::plugin().error()) return ERROR;
        else if(sql::plugin().emptyResult()) return FAIL;
        sql::plugin().useResult();
        return (front_end_info[PWD_VALUE_NAME].asString() == sql::plugin().last_row_result[0][0]) ? OK : FAIL;
    }

    return ERROR;
}

std::string Login::returnSuccess()
{
    Json::FastWriter writer;
    this->res_info[STA_VALUE_NAME] = OK;// status is 1:OK

    std::string usr_qry = "SELECT * FROM User_Info WHERE id = " + front_end_info[ID_VALUE_NAME].asString();// get usr info and return to front
    sql::plugin().query(usr_qry);
    sql::plugin().useResult();

    this->res_info[DATA_VALUE_NAME] = sql::plugin().resultToStyledJson();// get usr info from sql
    this->res_info[DATA_VALUE_NAME].removeMember(PWD_VALUE_NAME);// remove password value from result

    return writer.write(this->res_info);
}

std::string Login::returnError()
{
    Json::FastWriter writer;
    this->res_info[STA_VALUE_NAME] = ERROR;
    this->res_info[DATA_VALUE_NAME] = Json::nullValue;
    return writer.write(res_info);
}

std::string Login::returnFail()
{
    Json::FastWriter writer;
    this->res_info[STA_VALUE_NAME] = FAIL;
    this->res_info[DATA_VALUE_NAME] = Json::nullValue;

    return writer.write(res_info);
}

std::string Login::getResult()
{
    const int result = compare();
    if(result == OK) return returnSuccess();
    else if(result == FAIL) return returnFail();

    return returnError();
}



class Register
{
private:
    Json::Value data;
private:
    Register() {}
    int createRoot();
    int createAdmin();
    int creatNormal();
    int checkDuplicated();
public:
    static Register& singleton();
    void setData(const std::string&);
    void setData(const char*);
    int createAccount();
    std::string returnSuccess();
    std::string returnFail();
    std::string returnError();
    std::string returnDuplicated();

    ~Register() = default;
};

Register& Register::singleton()
{
    static Register handler;
    return handler;
}

int Register::createRoot()
{
    root usr;
    usr.load(this->data);

    sql::plugin().localConnect("root","","test"); // connect to the sql

    std::string qry = "INSERT INTO User_Into VALUES ( ";

    qry += GENERATE_STR(usr.name);
    qry += ",";
    qry += GENERATE_STR(usr.password);
    qry += ",";
    qry += GENERATE_STR(usr.type);
    qry += ",";
    qry += GENERATE_STR(usr.mail_address);
    qry += ",";
    qry += GENERATE_STR(usr.phone_number);
    qry += ",";
    qry += GENERATE_STR(usr.apartment);
    qry += ")";

    sql::plugin().query(qry);
    return (sql::plugin().error()) ? FAIL : OK;
}

#endif