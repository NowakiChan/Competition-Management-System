#ifndef EVENT
#define EVENT
/*design a class for basic event and event queue*/
#include<iostream>
#include<string>
#include<time.h>
#include<vector>

/*default define of different message categories*/
const std::string PRIVATE_MSG = "personal";
const std::string PUBLIC_MSG = "public";
const std::string GROUP_MSG = "group";
const std::string UNDEF = "undefine";
/*define end*/


class message
{
public:
    std::string context;// message text
    std::string category;// messgae category
    std::string source; // message source,it's a user's ID
    time_t message_t;// store the time that message was received
    bool state;// the message have been read or not
public:
    message() : category(UNDEF), source(UNDEF), message_t( time(NULL) ), state(false)
    {} 
    message(const std::string&,const std::string&,const std::string&);
    void set_read() { state = false; }// to set the message state
    void set_unread() { state = true; }
    bool is_read() const { return state == false; }// get message state
    ~message() = default;
};

message::message(const std::string& _context,const std::string& _category,const std::string& _source)
                : context(_context), source(_source), message_t( time(NULL) ), state(true)
{
    if(_category == PRIVATE_MSG || _category == PUBLIC_MSG || _category == GROUP_MSG)// if the provided category is illegal,then set it undef
        category = _category;
    else
        category = UNDEF;
}

class message_box
{
private:
    std::vector<message*> message_list;// using dynamic array to store the messsage
public:
    message_box() {}
    //int add_message(const message& new_msg);
    void add_message(message* new_msg) { message_list.push_back(new_msg); }
    /*query function*/
    message* latest_message() const { return message_list.back(); }
    message* first_message() const { return message_list.front(); }
    size_t unread_message() const;
    size_t number() const { return message_list.size(); }
    bool empty() const { return message_list.empty(); }
    message* operator [] (const size_t index) const;
    /*query function part end*/
    ~message_box();
};

message* message_box::operator [] (const size_t index) const
{
    if(index >= message_list.size())
        std::abort();
    return message_list[index];
}

size_t message_box::unread_message() const
{
    size_t cnt = 0;
    for(auto itr : this->message_list)
        if(!itr->is_read()) cnt++;
    return cnt;
}

message_box::~message_box()
{
    for(auto itr : message_list)
        delete itr;
}

#endif