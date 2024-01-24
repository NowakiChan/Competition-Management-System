#ifndef BASIC_USER
#define BASIC_USER
#include"event.h"

/*user type*/
const std::string ROOT = "root";
const std::string ADMIN = "admin";
const std::string NORMAL = "normal";
/*user type part end*/
const std::string UNDEF_PASS = "12345";
/*user level part*/
const std::string PRO = "professor";
const std::string ASO_PRO = "associate_professor";
const std::string TEAC = "teacher";
const std::string STU = "student";
/*user level part end*/


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

    virtual ~user() = default;
};
/*basic user part end*/
class root : public user
{
public:
    std::string apartment;
public:
    root() = default;
    root(const std::string&,const std::string&,const std::string&,const std::string&,const std::string&);
    root(const root& _root);
    ~root() = default;
};

root::root(const std::string& _name,const std::string& _password,const std::string& _apartment,
           const std::string& _phnumber = UNDEF,const std::string& _address = UNDEF)
          : user(_name,_password,ROOT,_phnumber,_address), apartment(_apartment)
{
    //user(_name,_password,ROOT,_phnumber,_address);
}

root::root(const root& _root) : apartment(_root.apartment)
{
    user(UNDEF,UNDEF_PASS,ROOT);
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
    admin(const admin& _admin);
    ~admin() = default;
};

admin::admin(const std::string& _name,const std::string& _password,const std::string& _college,
             const std::string& _profession,const std::string& _level,const std::string& _phnumber = UNDEF,
             const std::string& _address = UNDEF,double _amount = 0.0)
          : user(_name,_password,ADMIN,_phnumber,_address), college(_college), profession(_profession), work_amount(_amount)
{
    //user(_name,_password,ADMIN,_phnumber,_address); 
    if(_level != PRO && _level != ASO_PRO && _level != TEAC)
        level = UNDEF;
    else
        level = _level; 
}

admin::admin(const admin& _admin)
            : college(_admin.college), profession(_admin.profession), level(_admin.level), work_amount(0.0)
{
    user(UNDEF,UNDEF_PASS,ADMIN);
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
};

normal_user::normal_user(const std::string _name,const std::string _password,const std::string& _college,
                         const std::string& _level,const std::string& _grade = UNDEF,const std::string& _phnumber = UNDEF,
                         const std::string& _address = UNDEF,const double _amount = 0.0)
                        : user(_name,_password,NORMAL,_phnumber,_address), college(_college), level(_level), grade(NULL), work_amount(NULL)
{
    //user(_name,_password,NORMAL,_phnumber,_address);
    if(_level != PRO && _level != ASO_PRO && _level != TEAC && _level != STU)
        level = UNDEF;
    else
        level = _level; 
    if(level == PRO || level == ASO_PRO || level == TEAC)
        work_amount = new double(_amount);
    else if(level == STU)
        grade = new std::string(_grade);
}

normal_user::normal_user(const normal_user& _normal) : college(_normal.college), level(_normal.level)
{
    user(UNDEF,UNDEF_PASS,NORMAL);
    if(_normal.work_amount != NULL)
        this->work_amount = new double(0.0);
    else
        this->work_amount = NULL;
}

normal_user::~normal_user()
{
    if(work_amount != NULL)
        delete work_amount;
    if(grade != NULL)
        delete grade;
}
/*normal user part end*/
#endif