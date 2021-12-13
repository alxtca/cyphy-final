#include <iostream>
#include <string>
using namespace std;
#include "sqlite/sqlite3.h"

int height_to_return;//callback updates this variable, then getUserHeight will return this.

static int callback(void* NotUsed, int argc, char** argv, char** azColName);
int getUserHeight(std::string username, sqlite3* db);


int main()
{
    char* errorMessage;
    sqlite3* db;
    sqlite3_stmt* stmt;
    sqlite3_open("test.db", &db);

    //---------------- TABLE
    /*
    const char sql[] = "CREATE TABLE IF NOT EXISTS USER(name varchar(20), height INT)";
    int received = sqlite3_exec(db, sql, NULL, NULL, &errorMessage);
    if (received != SQLITE_OK) {
        cout << "error:" << errorMessage;
    }
    */

    //--------------CREATE (insert data)
    /*
    string user_name = "Balex";
    int height = 179;
    string db_insert = "INSERT INTO USER (name, height) VALUES('"+user_name+"', "+to_string(height)+")";
    //string db_insert = "INSERT INTO users (name, height) VALUES(";

    received = sqlite3_exec(db, db_insert.c_str(), NULL, NULL, &errorMessage);
    if (received != SQLITE_OK) {
        cout << "insert error:" << errorMessage;
    }
    */
    //------------READ (select data)

    string user_name = "Balex";
    int height = getUserHeight(user_name, db);
    cout << user_name << " height is " << height << endl;

    //-----------UPDATE
    // 
    //tbd

    return 0;
}

int getUserHeight(std::string user_name, sqlite3* db) {
    char* errorMessage;
    string db_select = "SELECT * FROM USER WHERE name = '" + user_name + "';";
    int received = sqlite3_exec(db, db_select.c_str(), callback, NULL, &errorMessage);

    if (received != SQLITE_OK) {
        cout << "select error:" << errorMessage;
    }

    return height_to_return;
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    if (argv > 0) {
        string temp = argv[1];
        height_to_return = stoi(temp);
    }
    else height_to_return = 0;

    return 0;
}

/*
* 
int sqlite3_exec(
  sqlite3*,                                  // An open database
const char* sql,                           // SQL to be evaluated
int (*callback)(void*, int, char**, char**),  // Callback function 
void*,                                    // 1st argument to callback
char** errmsg                              // Error msg written here
);

int sqlite3_prepare_v2(
  sqlite3 *db,            // Database handle 
const char* zSql,       // SQL statement, UTF-8 encoded 
int nByte,              // Maximum length of zSql in bytes. 
sqlite3_stmt** ppStmt,  // OUT: Statement handle 
const char** pzTail     // OUT: Pointer to unused portion of zSql 
);


*/