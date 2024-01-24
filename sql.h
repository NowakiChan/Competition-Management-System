#ifndef SQL
#define SQL
#include<iostream>
#include<vector>
#include<mysql/mysql.h>

/*field info store struct*/
class field
{
public:
    std::string name;// name of the sql_field
    size_t catalog_num;// crruent number of catalog in this field
    size_t length;// the legth of the field
public:
    field(const std::string& _name,const size_t _catalog,const size_t _length)
         : name(_name), catalog_num(_catalog), length(_length)
    {} 
};
/*end*/

class sql
{
private:
    MYSQL* sql_stream;
    MYSQL_RES* sql_res;
    MYSQL_FIELD* sql_field;
    MYSQL_ROW sql_row;
    sql() : sql_stream(NULL), sql_res(NULL) {}
public:
    static sql& plugin();// singleton plugin
    int query(const std::string&);// excute query in mysql
    std::vector<std::string> result_of_rows();// store the row result and return as a vector of string
    std::vector<field> result_of_fields();// store the field result and return as a vector of field-struct
    void local_connect(const std::string&,const std::string&,const std::string&);// connect local mysql
    void free_result();// must be used after finishing pocessing result of query: to free the space
    bool error();// check if error happen during last query,print the error message at the same time
    //void close_connect();
    std::string last_change();// return the message of last query
    int affect_rows();// return number of affected rows in last query 
    ~sql();// close connect
protected:
    std::string covertToString(MYSQL_ROW&,const int);// used in result(),covert MYSQL_ROW to vector of string
    bool isVaildField(const unsigned int index)// check if the crruent field holds a null value
    { return mysql_fetch_lengths(this->sql_res)[index] != 0; }
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
        if(isVaildField(cnt)) row_res += result[cnt];
        else row_res += " ";
        if(cnt < column - 1) row_res += "-";// each value is devided by a special mark : '-'
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
    this->sql_stream = mysql_init(this->sql_stream);// initialize
    mysql_real_connect(sql_stream,"localhost",user_name.c_str(),passwd.c_str(),db_name.c_str(),0,NULL,0);// connect to database
}

void sql::free_result()
{
    if(sql_res != NULL){
        mysql_free_result(sql_res);
        sql_res = NULL;
    }
}

int sql::query(const std::string& query_str)
{
    return mysql_real_query(sql_stream,query_str.c_str(),query_str.size());
}

std::vector<std::string> sql::result_of_rows()// get all result from last query,the result store in a vector<string>
{
    std::vector<std::string> res;
    if(sql_res == NULL)// protection, prevent from overusing of mysql_store_result(),which may result in seg-fault
        sql_res = mysql_store_result(sql_stream);
    if(sql_res != NULL){
        const int rows = mysql_num_rows(sql_res),fields = mysql_num_fields(sql_res);
        for(int offset = 0;offset < rows;offset++){// set offset to get the next row
            mysql_data_seek(sql_res,offset);
            sql_row = mysql_fetch_row(sql_res);
            res.push_back(covertToString(sql_row,fields));
        }
    }

    return res;
}

std::vector<field> sql::result_of_fields()
{
    std::vector<field> res;
    if(sql_res == NULL)// protection,similar to result()
        sql_res = mysql_store_result(sql_stream);
    if(sql_res != NULL){
        for(sql_field = mysql_fetch_field(sql_res);sql_field != NULL;sql_field = mysql_fetch_field(sql_res)){
            field column(sql_field->name,sql_field->catalog_length,sql_field->length);
            res.push_back(column);
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