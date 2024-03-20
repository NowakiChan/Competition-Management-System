#ifndef LOGIN
#define LOGIN
#include"../Basic Class/sql.h"
#include"../Basic Class/user.h"
/*login value name*/
#define GENERATE_STR(x) "'" + x + "'"
#define STA_VALUE_NAME "status"
#define DATA_VALUE_NAME "data"
#define PWD_VALUE_NAME "password"
#define ID_VALUE_NAME "id"
#define TYPE_VALUE_NAME "usertype"
#define USRNAME_VALUE_NAME "username"
/*login value name end*/
/*sql query for diffrent user group*/
#define ROOT_VALUE_LIST "(id,username,password,usertype,phone_number,email_address,college)"
#define ADMIN_VALUE_LIST "(id,username,password,usertype,phone_number,email_address,college,level,work_amount)"
#define STU_VALUE_LIST "(id,username,password,usertype,phone_number,email_address,college,profession,grade)"
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
        std::string info_qry = "SELECT password FROM User_Info WHERE id = ";
                    info_qry += GENERATE_STR( this->front_end_info[ID_VALUE_NAME].asString() );

        sql::plugin().query(info_qry);
        if(sql::plugin().error()) return ERROR;
        sql::plugin().useResult();
        if(sql::plugin().emptyResult()) return FAIL;

        Json::Value res = sql::plugin().resultToStyledJson();
        return (front_end_info[PWD_VALUE_NAME].asString() == res[PWD_VALUE_NAME].asString()) ? OK : FAIL;
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
    Json::Value dunplica_data;
    Json::Value similar_data;
private:
    Register() {}
    int createRoot();
    int createAdmin();
    int createNormal();
    //int creatAccount();
public:
    static Register& singleton();
    int setData(const std::string&);
    //void setData(const char*);
    std::string createAccount();
    std::string returnSuccess();
    std::string returnFail();
    std::string returnError();
    std::string returnDuplicated();

    ~Register() = default;
protected:
    bool legalData();
    int checkDuplicated();
    int checkSimilar();
};

bool Register::legalData()// judge whether the data has all necessary data
{
    return this->data.isMember(ID_VALUE_NAME) && this->data.isMember(USRNAME_VALUE_NAME) &&
           this->data.isMember(PWD_VALUE_NAME) && this->data.isMember(TYPE_VALUE_NAME);
}

int Register::checkDuplicated()
{
    sql::plugin().localConnect("root","","test");

    std::string qry = "SELECT * FROM User_Info WHERE id = ";
                qry += GENERATE_STR( this->data["id"].asString() );

    sql::plugin().useResult();
    if(sql::plugin().emptyResult()) return OK;

    this->dunplica_data = sql::plugin().resultToStyledJson();
    return FAIL;
}

int Register::checkSimilar()
{
    sql::plugin().localConnect("root","","test");

    std::string qry = "SELECT * FROM User_Info WHERE (";
    qry += "username=";
    qry += GENERATE_STR( this->data[USRNAME_VALUE_NAME].asString() );

    if(this->data.isMember("phone_number")){
        qry += " OR ";
        qry += "phone_number=";
        qry += GENERATE_STR( this->data["phone_number"].asString() );
    }
    if(this->data.isMember("email_address")){
        qry += " OR ";
        qry += "email_address=";
        qry += GENERATE_STR( this->data["email_address"].asString() );
    }
    qry += ")";

    sql::plugin().query(qry);
    if(sql::plugin().error()) return ERROR;

    sql::plugin().useResult();
    if(sql::plugin().emptyResult()) return OK;

    this->similar_data = sql::plugin().resultToStyledJson();
    return FAIL;
}

Register& Register::singleton()
{
    static Register handler;
    return handler;
}

int Register::setData(const std::string& front_end_data)
{
    Json::Reader reader;
    reader.parse(front_end_data,this->data);

    if(!legalData()) return FAIL;
    return checkDuplicated();
}

int Register::createRoot()
{
    sql::plugin().localConnect("root","","test"); // connect to the sql

    std::string qry = "INSERT INTO User_Info ";
                qry += ROOT_VALUE_LIST;
                qry += " VALUES (";

    qry += GENERATE_STR( this->data[ID_VALUE_NAME].asString() );
    qry += ",";
    qry += GENERATE_STR( this->data[USRNAME_VALUE_NAME].asString() );
    qry += ",";
    qry += GENERATE_STR( this->data[PWD_VALUE_NAME].asString() );
    qry += ",";
    qry += GENERATE_STR( this->data[TYPE_VALUE_NAME].asString() );
    qry += ",";
    qry += (this->data.isMember("phone_number")) ? GENERATE_STR( this->data["phone_number"].asString() ) : "NULL";
    qry += ",";
    qry += (this->data.isMember("email_address")) ? GENERATE_STR( this->data["email_address"].asString() ) : "NULL";
    qry += ",";
    qry += (this->data.isMember("college")) ? GENERATE_STR( this->data["college"].asString() ) : "NULL";
    qry += ")";

    sql::plugin().query(qry);
    return (sql::plugin().error()) ? ERROR : OK;
}

int Register::createAdmin()
{
    sql::plugin().localConnect("root","","test");

    std::string qry = "INSERT INTO User_Info ";
                qry += ADMIN_VALUE_LIST;
                qry += " VALUES (";

    qry += GENERATE_STR( this->data[ID_VALUE_NAME].asString() );
    qry += ",";
    qry += GENERATE_STR( this->data[USRNAME_VALUE_NAME].asString() );
    qry += ",";
    qry += GENERATE_STR( this->data[PWD_VALUE_NAME].asString() );
    qry += ",";
    qry += GENERATE_STR( this->data[TYPE_VALUE_NAME].asString() );
    qry += ",";
    qry += (this->data.isMember("phone_number")) ? GENERATE_STR( this->data["phone_number"].asString() ) : "NULL";
    qry += ",";
    qry += (this->data.isMember("email_address")) ? GENERATE_STR( this->data["email_address"].asString() ) : "NULL";
    qry += ",";
    qry += (this->data.isMember("college")) ? GENERATE_STR( this->data["college"].asString() ) : "NULL";
    qry += ",";
    qry += (this->data.isMember("level")) ? GENERATE_STR( this->data["level"].asString() ) : "NULL";
    qry += ",";
    qry += (this->data.isMember("work_amount")) ? this->data["work_amount"].asString() : "0.0";
    qry += ")";

    sql::plugin().query(qry);
    return (sql::plugin().error()) ? ERROR : OK;
}

int Register::createNormal()
{
    sql::plugin().localConnect("root","","test");

    std::string qry = "INSERT INTO User_Info ";
    if(this->data[TYPE_VALUE_NAME] == TEAC){
        qry += ADMIN_VALUE_LIST;
        qry += " VALUES (";

        qry += GENERATE_STR( this->data[ID_VALUE_NAME].asString() );
        qry += ",";
        qry += GENERATE_STR( this->data[USRNAME_VALUE_NAME].asString() );
        qry += ",";
        qry += GENERATE_STR( this->data[PWD_VALUE_NAME].asString() );
        qry += ",";
        qry += GENERATE_STR( this->data[TYPE_VALUE_NAME].asString() );
        qry += ",";
        qry += (this->data.isMember("phone_number")) ? GENERATE_STR( this->data["phone_number"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("email_address")) ? GENERATE_STR( this->data["email_address"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("college")) ? GENERATE_STR( this->data["college"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("level")) ? GENERATE_STR( this->data["level"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("work_amount")) ? this->data["work_amount"].asString() : "0.0";
        qry += ")";
    }
    else if(this->data[TYPE_VALUE_NAME] == STU){
        qry += STU_VALUE_LIST;
        qry += "VALUES (";

        qry += GENERATE_STR( this->data[ID_VALUE_NAME].asString() );
        qry += ",";
        qry += GENERATE_STR( this->data[USRNAME_VALUE_NAME].asString() );
        qry += ",";
        qry += GENERATE_STR( this->data[PWD_VALUE_NAME].asString() );
        qry += ",";
        qry += GENERATE_STR( this->data[TYPE_VALUE_NAME].asString() );
        qry += ",";
        qry += (this->data.isMember("phone_number")) ? GENERATE_STR( this->data["phone_number"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("email_address")) ? GENERATE_STR( this->data["email_address"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("college")) ? GENERATE_STR( this->data["college"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("profession")) ? GENERATE_STR( this->data["profession"].asString() ) : "NULL";
        qry += ",";
        qry += (this->data.isMember("grade")) ? GENERATE_STR( this->data["grade"].asString() ) : 
                GENERATE_STR( generateGrade(this->data[ID_VALUE_NAME].asString()) ); 
        qry += ")";
    }

    sql::plugin().query(qry);
    return (sql::plugin().error()) ? ERROR : OK;
}

std::string Register::returnSuccess()
{
    Json::FastWriter writer;
    Json::Value res;
    res["status"] = OK;
    res["data"] = Json::nullValue;

    return writer.write(res);
}

std::string Register::returnError()
{
    Json::FastWriter writer;
    Json::Value res;
    res["status"] = ERROR;
    res["data"] = Json::nullValue;

    return writer.write(res);
}

std::string Register::returnFail()
{
    Json::FastWriter writer;
    Json::Value res;
    res["status"] = FAIL;
    res["data"] = this->dunplica_data;

    return writer.write(res);
}

std::string Register::createAccount()
{
    int status = ERROR;

    if(this->data[TYPE_VALUE_NAME].asString() == ROOT)
        status = this->createRoot();
    else if(this->data[TYPE_VALUE_NAME].asString() == ADMIN)
        status = this->createAdmin();
    else if(this->data[TYPE_VALUE_NAME].asString() == STU || this->data[TYPE_VALUE_NAME].asString() == TEAC)
        status = this->createNormal();

    return (status == ERROR) ? this->returnError() : this->returnSuccess();
}

#endif