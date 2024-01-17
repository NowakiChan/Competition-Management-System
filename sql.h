#ifndef SQL
#define SQL
#include<iostream>
#include<vector>
#include<mysql/mysql.h>

class sql
{
private:
    MYSQL* sql_stream;
    MYSQL_RES* sql_res;
    MYSQL_ROW sql_row;
    sql() : sql_stream(NULL) {}
public:
    static sql& plugin();
    int query(const std::string&);
    std::vector<std::string> result();
    void local_connect(const std::string&,const std::string&,const std::string&);
    bool error();
    //bool empty_result();
    std::string last_change();
    int affect_rows();
    ~sql();
protected:
    std::string covertToString(MYSQL_ROW&,const int);
};

std::string sql::last_change()
{
    if(mysql_info(sql_stream) != NULL)
        return std::string(mysql_info(sql_stream));
    return std::string("no info from last change");
}

int sql::affect_rows()
{
    return mysql_affected_rows(sql_stream);
}

std::string sql::covertToString(MYSQL_ROW& result,const int column)// covert single row to a string
{
    std::string row_res;
    for(int cnt = 0;cnt < column;cnt++){
        row_res += result[cnt];
        if(cnt < column - 1) row_res += "-";
    }
    return row_res;
}

sql& sql::plugin()
{
    static sql connector;
    return connector;
}

void sql::local_connect(const std::string& user_name,const std::string& passwd,const std::string& db_name)
{
    this->sql_stream = mysql_init(this->sql_stream);
    mysql_real_connect(sql_stream,"localhost",user_name.c_str(),passwd.c_str(),db_name.c_str(),0,NULL,0);
}

int sql::query(const std::string& query_str)
{
    return mysql_real_query(sql_stream,query_str.c_str(),query_str.size());
}

std::vector<std::string> sql::result()// get all result from last query,the result store in a vector<string>
{
    std::vector<std::string> res;
    sql_res = mysql_store_result(sql_stream);
    if(sql_res != NULL){
        const int rows = mysql_num_rows(sql_res),fields = mysql_num_fields(sql_res);
        for(int offset = 0;offset < rows;offset++){
            mysql_data_seek(sql_res,offset);
            sql_row = mysql_fetch_row(sql_res);
            res.push_back(covertToString(sql_row,fields));
        }
    }
    return res;
}

bool sql::error()
{
    if(*mysql_error(sql_stream)){
        std::cout<<"error "<<mysql_errno(sql_stream)<<std::endl;
        std::cout<<mysql_error(sql_stream)<<std::endl;
        return true;
    }

    //std::cout<<"no error from last query\n";
    return false;
}

sql::~sql()
{
    if(sql_stream != NULL)
        mysql_close(sql_stream);
}


#endif