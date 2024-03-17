#ifndef COMPETITION
#define COMPETITION
#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>
#include<vector>


class competition
{
public:
    std::string holding_year;
    std::string start_time;
    std::string end_time;
    std::string belong_department;
    std::string admin_name;
public:
    competition(const char* _year,const char* _start,const char* _end,
                const char* _department,const char* _admin)
               : holding_year(_year), start_time(_start), end_time(_end),
                 belong_department(_department), admin_name(_admin)
    {}
    competition(const competition&);
    Json::Value toJson();
    std::string toJsonStyledString();
    ~competition() = default;
};

class competition_list
{
private:
    std::vector<competition*> events;
public:
    std::string name;
    std::string id;
    std::string category;
    std::string state;
public:
    competition_list(const char*,const char*,const char*,const char*);
    competition_list(const competition&);
    competition_list(const competition_list&);
    void addEvent();
    size_t number() { return events.size(); }
    competition operator [] (const size_t);
    Json::Value toJson();
    std::string toJsonStyledString();
    ~competition_list();
};
#endif