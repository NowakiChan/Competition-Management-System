#ifndef EVENT
#define EVENT
/*design a class for basic event and event queue*/
#include<iostream>
#include<string>
#include<time.h>
#include<vector>
#include<fstream>
#include<jsoncpp/json/json.h>
/*macro for state*/
#define READ 0
#define UNREAD 1
#define UNINIT 2
#define INVAILD -1
/*member name macro*/
#define CONTEXT "context"
#define CATEGORY "category"
#define SOURCE "source"
#define DEST "dest"
#define TIME "time"
#define STATE "state"
/*default define of different message categories*/
#define PRIVATE_MSG "personal"
#define PUBLIC_MSG "public"
#define GROUP_MSG "group"
#define SYSTEM_MSG "system"
#ifndef UNDEF
#define UNDEF "undefined"
#endif
/*macro function for message::state and message::category*/
#define VAILD_STATE(x) (x == READ || x == UNREAD || x == UNINIT)
/*define end*/

/*non-menber function declaration*/
std::string getImmediateTime();
/*end*/



class message
{
public:
    std::string context;// message text
    std::string category;// messgae category
    std::string source; // message source,it's a user's ID
    std::string dest;
    std::string message_t;// store the time that message was received
    int state;// the message have been read or not
public:
    message() : category(UNDEF), source(UNDEF), dest(UNDEF), message_t( getImmediateTime() ), state(UNINIT)
    {} 
    message(const std::string&,const std::string&,const std::string&,const std::string&);
    message(const std::string&,const std::string&,const std::string&,const std::string&,
            const std::string&,const int);
    void set_read() { state = READ; }// to set the message state
    void set_unread() { state = UNREAD; }
    bool is_read() const { return state == READ; }// get message state
    Json::Value toJson() const;
    ~message() = default;
};

message::message(const std::string& _context,const std::string& _category,const std::string& _source,const std::string& _dest)
                : context(_context), source(_source), dest(_dest), message_t( getImmediateTime() ), state(UNREAD)
{
    if(_category == PRIVATE_MSG || _category == PUBLIC_MSG ||
       _category == GROUP_MSG || _category == SYSTEM_MSG)// if the provided category is illegal,then set it undef
        category = _category;
    else
        category = UNDEF;
}

message::message(const std::string& _context,const std::string& _category,const std::string& _source,const std::string& _dest,
                 const std::string& _time,const int _state)
                : context(_context), category(_category), source(_source), dest(_dest), message_t(_time)
{
    if(VAILD_STATE(_state))
        state = _state;
    else
        state = INVAILD;
}

Json::Value message::toJson() const
{
    Json::Value msg;
    msg[CONTEXT] = this->context;
    msg[CATEGORY] = this->category;
    msg[SOURCE] = this->source;
    msg[DEST] = this->dest;
    msg[TIME] = this->message_t;
    msg[STATE] = this->state;
    return msg;
}

class message_box
{
private:
    std::vector<message*> message_list;// using dynamic array to store the messsage
public:
    message_box() {}
    //int add_message(const message& new_msg);
    void add_message(message* new_msg) { message_list.push_back(new_msg); }
    void set_all_read();
    /*query function*/
    message latest_message() const { return *message_list.back(); }
    message first_message() const { return *message_list.front(); }
    size_t unread_message() const;
    size_t number() const { return message_list.size(); }
    bool empty() const { return message_list.empty(); }
    message operator [] (const size_t index) const;
    /*query function part end*/
    void load(const std::string&);
    void save(const std::string&) const;
    ~message_box();
protected:
    void loadFromJson(const Json::Value&);
    const Json::Value saveToJson() const;
};

message message_box::operator [] (const size_t index) const
{
    if(index >= message_list.size())
        std::abort();
    return *message_list[index];
}

void message_box::set_all_read()
{
    for(auto this_msg : message_list)
        if(VAILD_STATE(this_msg->state) && this_msg->state == UNREAD)
            this_msg->state = READ;
}

void message_box::loadFromJson(const Json::Value& obj)
{
    for(int i = 0;i < obj.size();i++){
        int _state = (obj[i].isMember(STATE) && VAILD_STATE(obj[i][STATE].asInt())) ? obj[i][STATE].asInt() : INVAILD;
        if(_state == INVAILD) continue;// skip unvaild message

        std::string _context,_category,_source,_dest,_time;
        if(obj[i].isMember(CONTEXT))
            _context = obj[i][CONTEXT].asString();
        else _state = INVAILD;
        if(obj[i].isMember(CATEGORY))
            _category = obj[i][CATEGORY].asString();
        else _state = INVAILD;
        if(obj[i].isMember(SOURCE))
            _source = obj[i][SOURCE].asString();
        else _state = INVAILD;
        if(obj[i].isMember(DEST))
            _dest = obj[i][DEST].asString();
        else _state = INVAILD;
        if(obj[i].isMember(TIME))
            _time = obj[i][TIME].asString();
        else _state = INVAILD;

        add_message( new message(_context,_category,_source,_dest,_time,_state) );
    }
}

const Json::Value message_box::saveToJson() const
{
    Json::Value result;
    result = Json::Value(Json::arrayValue);

    for(auto single_message : this->message_list){
        result.append( single_message->toJson() );
    }

    return result;
}

size_t message_box::unread_message() const
{
    size_t cnt = 0;
    for(auto itr : this->message_list)
        if(!itr->is_read()) cnt++;
    return cnt;
}

void message_box::load(const std::string& file_path)
{
    std::ifstream fin;
    Json::Reader reader;
    Json::Value result;
    fin.open(file_path);
    if(fin.is_open()){
        reader.parse(fin,result,false);
        loadFromJson(result);
    }
}

void message_box::save(const std::string& dest_path) const
{
    std::ofstream fout;
    fout.open(dest_path);
    if(fout.is_open()){
        fout<<saveToJson().toStyledString();
    }
}

message_box::~message_box()
{
    for(auto itr : message_list)
        delete itr;
}

/*non-menber function*/
std::string getImmediateTime()
{
    time_t immediate_t = time(NULL);
    return std::string( ctime(&immediate_t) );
}

std::ostream& operator << (std::ostream& os,const message& msg)
{
    os << msg.category << " message :" << std::endl;
    os << msg.context << std::endl;
    os << "From :" << msg.source << " At " << msg.message_t << std::endl;
    os << "Status : "<< msg.state << std::endl;
    return os;
}
/*end*/
#endif