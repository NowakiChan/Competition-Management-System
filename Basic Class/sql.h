#ifndef SQL
#define SQL
#include<iostream>
#include<vector>
#include<mysql/mysql.h>
#include<jsoncpp/json/json.h>

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

typedef std::vector<std::vector<std::string> > Row;
typedef std::vector<field> Column;

class sql
{
private:
    MYSQL* sql_stream;
    MYSQL_RES* sql_res;
    MYSQL_FIELD* sql_field;
    MYSQL_ROW sql_row;
    bool is_empty_result;// to check if last query returns an empty result
    sql() : sql_stream(NULL), sql_res(NULL) , error_code(0) {}
public:
    Row last_row_result;
    Column last_field_result;
    unsigned int error_code;
public:
    static sql& plugin();// singleton plugin
    int query(const std::string&);// excute query in mysql
    void localConnect(const std::string&,const std::string&,const std::string&);// connect local mysql
    void useResult();
    bool error();// check if error happen during last query,print the error message at the same time
    //void close_connect();
    bool emptyResult() { return this->is_empty_result; }
    std::string lastChange();// return the message of last query
    int affectRows();// return number of affected rows in last query 
    std::string resultToJson();// new member
    Json::Value resultToStyledJson();// new member
    ~sql();// close connect
protected:
    void freeResult();// must be used after finishing pocessing result of query: to free the space
    void resultOfFields();// store the field result and return as a vector of field-struct
    void resultOfRows();// store the row result and return as a vector of string
    std::vector<std::string> covertToArray(MYSQL_ROW&,const int);// used in result(),covert MYSQL_ROW to vector of string
    bool isVaildField(const unsigned int index)// check if the crruent field holds a null value
    { return mysql_fetch_lengths(this->sql_res)[index] != 0; }
    Json::Value toJsonObj(std::vector<std::string>&);
    bool checkRowResult() { return mysql_num_rows(this->sql_res) != 0; }// check if query returns an empty result
    bool checkFieldResult() { return mysql_field_count(this->sql_stream) != 0; }// check if there are expected results
};

std::string sql::lastChange()
{
    if(mysql_info(sql_stream) != NULL)
        return std::string(mysql_info(sql_stream));
    return std::string("no info from last change");
}

int sql::affectRows()
{
    return mysql_affected_rows(sql_stream);
}

Json::Value sql::toJsonObj(std::vector<std::string>& single_row_res)
{
    Json::Value res;
    if(this->last_field_result.size() != single_row_res.size())
        std::abort();
    for(size_t index = 0;index < this->last_field_result.size();index++){
         res[last_field_result[index].name] = (single_row_res[index].size() != 0)
                                              ? single_row_res[index] : Json::Value(Json::nullValue);
    }
    return res;
}

std::vector<std::string> sql::covertToArray(MYSQL_ROW& result,const int column)// covert single row to a string
{
    std::vector<std::string> row_res;
    for(int cnt = 0;cnt < column;cnt++){
        if(isVaildField(cnt)) row_res.push_back(result[cnt]);
        else row_res.push_back(std::string(""));
        //if(cnt < column - 1) row_res += "-";// each value is devided by a special mark : '-'
    }
    return row_res;
}

sql& sql::plugin()
{
    static sql connector;
    return connector;
}

void sql::localConnect(const std::string& user_name,const std::string& passwd,const std::string& db_name)
{
    this->sql_stream = mysql_init(this->sql_stream);// initialize
    mysql_real_connect(sql_stream,"localhost",user_name.c_str(),passwd.c_str(),db_name.c_str(),0,NULL,0);// connect to database
}

void sql::freeResult()
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

void sql::resultOfRows()// get all result from last query,the result store in public member last_row_res
{
    Row res;
    if(sql_res == NULL && checkFieldResult()){// protection, prevent from overusing of mysql_store_result(),which may result in seg-fault
        sql_res = mysql_store_result(sql_stream);
        is_empty_result = !checkRowResult();// determine whether query returns an empty result 
    }                                       // note that function chckRowResult() returns true when result is unempty
    else if(sql_res != NULL && checkFieldResult()){
        const int rows = mysql_num_rows(sql_res),fields = mysql_num_fields(sql_res);
        for(int offset = 0;offset < rows;offset++){// set offset to get the next row
            mysql_data_seek(sql_res,offset);
            sql_row = mysql_fetch_row(sql_res);
            res.push_back(covertToArray(sql_row,fields));
        }
    }

    last_row_result = res;
}

void sql::useResult()
{
    this->resultOfRows();
    this->resultOfFields();
    this->freeResult();
}

void sql::resultOfFields()
{
    std::vector<field> res;
    if(sql_res == NULL && checkFieldResult())// protection,similar to result()
        sql_res = mysql_store_result(sql_stream);
    else if(sql_res != NULL && checkFieldResult()){
        for(sql_field = mysql_fetch_field(sql_res);sql_field != NULL;sql_field = mysql_fetch_field(sql_res)){
            field column(sql_field->name,sql_field->catalog_length,sql_field->length);
            res.push_back(column);
        }
    }

    last_field_result = res;
}

std::string sql::resultToJson()
{
    Json::FastWriter fwriter;
    return fwriter.write(this->resultToStyledJson());
}


Json::Value sql::resultToStyledJson()
{
    Json::Value res;
    if(this->last_row_result.size() == 1){
        return toJsonObj(this->last_row_result[0]);
    }
    else if(this->last_row_result.size() > 1){
        res = Json::Value(Json::arrayValue);
        for(auto this_row : this->last_row_result)
            res.append( toJsonObj(this_row) );
    }
    return res;
}

bool sql::error()
{
    if(*mysql_error(sql_stream)){
        error_code = mysql_errno(sql_stream);
        //std::cout<<"error "<<mysql_errno(sql_stream)<<std::endl;
        //std::cout<<mysql_error(sql_stream)<<std::endl;
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