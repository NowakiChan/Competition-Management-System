#ifndef LOGIN
#define LOGIN
#include"../Basic Class/sql.h"
#include"../Basic Class/user.h"

#ifndef ERROR
#define ERROR -1
#endif
#ifndef OK
#define OK 1
#endif
#ifndef FAIL
#define FAIL 0
#endif

#define GENERATE_STR(x) "'x'"
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
    Login singleton;
    return singleton;
}

void Login::initiate(const std::string& usr_str)
{
    Json::Reader processer;
    processer.parse(usr_str,this->front_end_info,false);

    sql::plugin().localConnect("root","","test");
}

void Login::initiate(const char* usr_str)
{
    Json::Reader processer;
    processer.parse(usr_str,this->front_end_info,false);
}

int Login::compare()
{
    if(front_end_info.isMember("id") && front_end_info.isMember("password")){
        std::string info_qry = "SELECT password FROM User_Info WHERE id = " + front_end_info["id"].asString();
        sql::plugin().query(info_qry);
        if(sql::plugin().error()) return ERROR;

        sql::plugin().useResult();
        return (front_end_info["password"].asString() == sql::plugin().last_row_result[0][0]) ? OK : FAIL;
    }

    return ERROR;
}

std::string Login::returnSuccess()
{
    Json::FastWriter writer;
    this->res_info["ststus"] = OK;

    std::string usr_qry = "SELECT * FROM User_Info WHERE id = " + front_end_info["id"].asString();
    sql::plugin().query(usr_qry);
    sql::plugin().useResult();
    this->res_info["data"] = sql::plugin().resultToStyledJson();

    return writer.write(this->res_info);
}

std::string Login::returnError()
{
    Json::FastWriter writer;
    this->res_info["status"] = ERROR;
    this->res_info["data"] = Json::nullValue;
    return writer.write(res_info);
}

std::string Login::returnFail()
{
    Json::FastWriter writer;
    this->res_info["status"] = FAIL;
    this->res_info["data"] = Json::nullValue;

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
    Register handler;
    return handler;
}

int Register::createRoot()
{
    root usr;
    usr.load(this->data);

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