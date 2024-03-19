#ifndef BASIC_USER
#define BASIC_USER
#include"event.h"
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
const std::string UNDEF_PASS = "12345";
/*user level part*/
#define PRO "professor"
#define ASO_PRO "associate_professor"
#define TEAC "teacher"
#define STU "student"
/*user level part end*/
#define VAILD_LEVEL(str) (str == PRO || str == ASO_PRO || str == TEAC || str == STU)
#define WITH_AMOUNT(str) (str == PRO || str == ASO_PRO || str == TEAC)
#define WITH_GRADE(str) (str == STU)


/*each class should provide ctor that provide the unnecessary value with a default value*/
/*and the unnecessary value should be put at the end of the parameter list*/
/*provided function overload that support c-style string*/
class user
{
public:
    std::string name;
    std::string password;
    std::string phone_number;// new menber
    std::string mail_address;// new menber
    std::string type;
    message_box mail;
public:
    user() = default;
    user(const user& _user) : name(UNDEF), password(UNDEF_PASS), type(_user.type), phone_number(UNDEF), mail_address(UNDEF) {}
    user(const std::string& _name,const std::string& _password,const std::string& _type,
         const std::string& _phnumber = UNDEF,const std::string& _address = UNDEF)
        : name(_name), password(_password), type(_type), phone_number(_phnumber), mail_address(_address)
    {}
    virtual void load(const char*);
    virtual void load(const Json::Value& value) { loadFromJson(value); }
    //virtual void load(const std::string&);
    //virtual void load();

    virtual ~user() = default;
protected:
    virtual void loadFromJson(const Json::Value&) {}
};

void user::load(const char* file_path)
{
    std::ifstream fin;
    fin.open(file_path);
    if(fin.is_open()){
        Json::Reader reader;
        Json::Value info;
        reader.parse(fin,info);
        loadFromJson(info);
    }
}
/*basic user part end*/
class root : public user
{
public:
    std::string apartment;
public:
    root() = default;
    root(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&);
    root(const root& _root) : user(UNDEF,UNDEF,ROOT), apartment(_root.apartment)
    {}
    ~root() = default;
protected:
    virtual void loadFromJson(const Json::Value&);
};

root::root(const std::string& _name,const std::string& _password,const std::string& _apartment,
           const std::string& _phnumber = UNDEF,const std::string& _address = UNDEF)
          : user(_name,_password,ROOT,_phnumber,_address), apartment(_apartment)
{
    //user(_name,_password,ROOT,_phnumber,_address);
}

void root::loadFromJson(const Json::Value& user_info)
{
    if(user_info.isMember(NAME)) this->name = user_info[NAME].asString();
    if(user_info.isMember(PWD)) this->password = user_info[PWD].asString();
    if(user_info.isMember(PHNUMBER)) this->phone_number = user_info[PHNUMBER].asString();
    if(user_info.isMember(ADDRESS)) this->mail_address = user_info[ADDRESS].asString();
    if(user_info.isMember(APARTMENT)) this->apartment = user_info[APARTMENT].asString();
}

/*root part end*/
class admin : public user
{
public:
    std::string college;
    std::string profession;
    std::string level;
    double work_amount;
public:
    admin() = default;
    admin(const std::string&,const std::string&,const std::string&,
          const std::string&,const std::string&,const std::string&,
          const std::string&,const double);
    admin(const admin& _admin): user(UNDEF,UNDEF,ADMIN),college(_admin.college),
                                profession(_admin.profession), level(_admin.level), work_amount(0.0)
    {}
    ~admin() = default;
protected:
    virtual void loadFromJson(const Json::Value&);
};

admin::admin(const std::string& _name,const std::string& _password,const std::string& _college,
             const std::string& _profession,const std::string& _level,const std::string& _phnumber = UNDEF,
             const std::string& _address = UNDEF,double _amount = 0.0)
          : user(_name,_password,ADMIN,_phnumber,_address), college(_college), profession(_profession), work_amount(_amount)
{
    //user(_name,_password,ADMIN,_phnumber,_address); 
    if(VAILD_LEVEL(_level))
        level = UNDEF;
    else
        level = _level; 
}

void admin::loadFromJson(const Json::Value& user_info)
{
    if(user_info.isMember(NAME)) this->name = user_info[NAME].asString();
    if(user_info.isMember(PWD)) this->password = user_info[PWD].asString();
    if(user_info.isMember(PHNUMBER)) this->phone_number = user_info[PHNUMBER].asString();
    if(user_info.isMember(ADDRESS)) this->mail_address = user_info[ADDRESS].asString();
    if(user_info.isMember(COLLEGE)) this->college = user_info[APARTMENT].asString();
    if(user_info.isMember(PROFESSION)) this->profession = user_info[PROFESSION].asString();
    if(user_info.isMember(LEVEL) && WITH_AMOUNT(user_info[LEVEL].asCString())){
        this->level = user_info[LEVEL].asString();
        this->work_amount = (user_info.isMember(AMOUNT)) ? user_info[AMOUNT].asDouble() : 0.0;
    }
}

/*admin part end*/
class normal_user : public user
{
public:
    std::string college;
    std::string level;
private:
    std::string* grade;// new menber
    double* work_amount;
public:
    normal_user() : grade(NULL), work_amount(NULL) {}
    normal_user(const std::string,const std::string,const std::string&,
                const std::string&,const std::string&,const std::string&,
                const std::string&,const double);
    normal_user(const normal_user& _normal);
    std::string Grade() const { return (grade == NULL) ? std::string("") : *grade; }
    double Work() const { return (work_amount == NULL) ? -1.0 : *work_amount; }
    ~normal_user();
protected:
    virtual void loadFromJson(const Json::Value&);
};

normal_user::normal_user(const std::string _name,const std::string _password,const std::string& _college,
                         const std::string& _level,const std::string& _grade = UNDEF,const std::string& _phnumber = UNDEF,
                         const std::string& _address = UNDEF,const double _amount = 0.0)
                        : user(_name,_password,NORMAL,_phnumber,_address), college(_college), level(_level), grade(NULL), work_amount(NULL)
{
    //user(_name,_password,NORMAL,_phnumber,_address);
    if(VAILD_LEVEL(_level))
        level = _level;
    else
        level = UNDEF; 
    if(WITH_AMOUNT(_level))
        work_amount = new double(_amount);
    else if(WITH_GRADE(_level))
        grade = new std::string(_grade);
}

normal_user::normal_user(const normal_user& _normal)
                        : user(UNDEF,UNDEF,NORMAL), college(_normal.college), level(_normal.level),
                          work_amount(NULL), grade(NULL)
{
    //user(UNDEF,UNDEF_PASS,NORMAL);
    if(WITH_AMOUNT(_normal.level))
        this->work_amount = new double(0.0);
    else if(WITH_GRADE(_normal.level))
        this->grade = new std::string(*_normal.grade);
}

normal_user::~normal_user()
{
    if(work_amount != NULL)
        delete work_amount;
    if(grade != NULL)
        delete grade;
}

void normal_user::loadFromJson(const Json::Value& user_info)
{
    if(user_info.isMember(NAME)) this->name = user_info[NAME].asString();
    if(user_info.isMember(PWD)) this->password = user_info[PWD].asString();
    if(user_info.isMember(PHNUMBER)) this->phone_number = user_info[PHNUMBER].asString();
    if(user_info.isMember(ADDRESS)) this->mail_address = user_info[ADDRESS].asString();
    if(user_info.isMember(COLLEGE)) this->college = user_info[APARTMENT].asString();
    if(user_info.isMember(LEVEL) && WITH_AMOUNT(user_info[LEVEL].asCString())){
        this->level = user_info[LEVEL].asString();
    }
    if(WITH_AMOUNT(this->level)) *this->work_amount = user_info[AMOUNT].asDouble();
    else if(WITH_GRADE(this->level)) *this->grade = user_info[GRADE].asString();
}
/*normal user part end*/
#endif