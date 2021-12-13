#include "database.h"
Database* Database::p_this_db = NULL;



int Database::callback(void* NotUsed, int num_results, char** values, char** columns)
{
    int i;
    for (i = 0; i < num_results; i++) {
        printf("%s = %s\n", columns[i], values[i] ? values[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int Database::callback_username_Exist(void* NotUsed, int args, char** argv, char** azColName){
    if(args > 0){
        p_this_db->user_exist = true; //Yes
        std::cout << "argv > 0 " << std::endl;
    }
    else {
        p_this_db->user_exist = false; //No
        std::cout << "argv !> 0 "<< std::endl;
    }
    
    return 0;
}

bool Database::username_Exist(std::string user_n) {
    user_exist = false;
    char* errMsg;
    int exit = open_database();
    std::cout << "going to check user name in db: " << user_n << std::endl;
    if (!exit) {//Database opened succsessfully
        std::cout << "inne i if(!exit): " << std::endl;
        std::string sql = "SELECT * FROM USER WHERE NAME = '"+user_n+"'";
        int rc = sqlite3_exec(DB, sql.c_str(), callback_username_Exist, 0, &errMsg);
        if (rc != SQLITE_OK){
            std::cout << "select error" << errMsg<< std::endl;
        }
    }
    close_database();
    std::cout << "user_exist "<< p_this_db->user_exist << std::endl;
    return p_this_db->user_exist;

}

int Database::save_callback(void* NotUsed, int num_results, char** values, char** columns)
{
    if (num_results == 2 ) {
      
        p_this_db->name = values[0]; // name
        p_this_db->height = values[1]; // height
        std::cout << "name " << p_this_db->name << " height " << p_this_db->height << std::endl;

    }
    else
    {
        p_this_db->name = ""; 
        p_this_db->height = "";
        std::cout << "ERROR save_callback" << std::endl;
    }
  
    return 0;
}

void Database::send_to_lcd() { // Write to lcd code
    // Write to lcd code
    int i2c_bus;
    i2c_bus = lcd1602Init(1, 0x27); // parameters: channel, address
    if (i2c_bus)
    {
        printf("Initialization failed; aborting...\n");
        printf("IS i2c com open on device?\n");
    }
    //Show name:
    lcd1602SetCursor(0, 0); // default position, character 0, line 0
    char line1ToPrint[p_name.size() + 1]{};   // prepare a print array
    p_name.copy(line1ToPrint, p_name.size() + 1);   // copy name-string to print array
    line1ToPrint[p_name.size()] = '\0';       // adding stop character
    lcd1602WriteString(line1ToPrint);    // write to top line
    //Show height:
    lcd1602SetCursor(0, 1); // change line position, character 0, line 1
    char line2ToPrint[p_height.size() + 1]{};     // reset print array
    p_height.copy(line2ToPrint, p_height.size() + 1);   // copy height-string to print array
    line2ToPrint[p_height.size()] = '\0';       // adding stop character
    lcd1602WriteString(line2ToPrint);    // write to bottom line
   
}

int Database::open_database()
{
    int exit = 0;
    exit = sqlite3_open(database_name.c_str(), &DB); //opens the database
    test_open(exit, "DB Open: "); //Check if database opened succsessfully
    
    return exit;
}

void Database::test_open(int exit, std::string success_message)
{
    if (exit != SQLITE_OK) {// The database interaction failed
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message); 
    }
    else // Database interaction succsessful
        std::cout << success_message; 
}


void Database::create_table()
{
    int exit = open_database();

    if (!exit) { //Database opened succsessfully
        /*SQL statment for table creation*/
        exit = sqlite3_exec(DB, "CREATE TABLE USER(" \
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"\
            "NAME TEXT NOT NULL,"\
            "HEIGHT INT NOTT NULL,"\
            "TIME TIMESTAMP WITHOUT TIME ZONE); ", callback, 0, &error_message);

        test_open(exit, "Table created successfully"); //Check if table created successfully
    }
    close_database();

}

void Database::write_user(std::string name, int height)
{
    int exit = open_database();

    if (!exit) {//Database opened succsessfully
        /*Create string for sql Query*/
        std::string sql = "INSERT INTO USER (NAME,HEIGHT,TIME) VALUES('"+name+"',"+std::to_string(height)+",'"+__TIMESTAMP__+"')";
        /*SQL statment for inserting user into table*/
        exit = sqlite3_exec(DB,sql.c_str(), callback, 0, &error_message);
        test_open(exit, "Innsert to table successfully");
    }
    
    close_database();
}

void Database::read_user(std::string name)
{
    int exit = open_database();
    if (!exit) {//Database opened succsessfully
        /*Create string for sql Query*/
        std::string sql = "SELECT NAME,HEIGHT FROM USER WHERE NAME ='" + name + "'";
        /*SQL statment for finding user in table*/
        exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &error_message);
        test_open(exit, "Read from table successfull");
    }
    close_database();
}

void Database::get_last_user(void)
{
    int exit = open_database();
    if (!exit) {//Database opened succsessfully
        /*Create string for sql Query*/
        std::string sql = "SELECT NAME,HEIGHT FROM USER ORDER BY ID DESC LIMIT 1";
        /*SQL statment for finding user in table*/
        exit = sqlite3_exec(DB, sql.c_str(), save_callback, 0, &error_message);
        test_open(exit, "Get data from table successfull");
    }
    close_database();

}

void Database::print_to_lcd()
{
    if (strcmp(language, "EN")==0) {
        p_name = "User  : ";
        p_height = "Height: ";
    }
    else if (strcmp(language, "NO")==0) {
        p_name = "Bruker: ";
        p_height = "H�yde : ";

    }
    else {
        std::cout << "ERROR: database/save_callback/language" << std::endl;
    }

    p_name.append(name);    // eks: "Bruker: username"
    p_height.append(height);    // eks: "H�yde : 180"

    p_height.append(" cm");        // eks: "H�yde : 180 cm"

    //------test of values--------
    std::cout << std::endl << p_name << std::endl << p_height << std::endl << std::endl;
    //------test of values--------
    send_to_lcd();
    p_name = "";
    p_height = "";

}

void Database::close_database()
{
    int exit = sqlite3_close(DB); //Close the database
    test_open(exit, " :DB Closed\n");//Check if database closed successfully


}

void Database::view_database()
{

    int exit = open_database();

    if (!exit) {//Database opened succsessfully
        std::cout <<"View Database "<<std::endl;
        /*SQL statment for printing all users in table*/
        sqlite3_exec(DB, "SELECT NAME,HEIGHT FROM USER", callback, 0, &error_message);
    }
    close_database();
}

void Database::clear_database()
{
    int exit = open_database();

    if (!exit) {//Database opened succsessfully
        /*SQL statment for deleting table*/
        exit = sqlite3_exec(DB, "DROP TABLE USER", callback, 0, &error_message);
        test_open(exit, "Table removed successfully");
    }
    close_database();
}


bool Database::used_user_name(std::string n)
{
    int exit = open_database();
    if (!exit) {//Database opened succsessfully
        /*Create string for sql Query*/
        std::string sql = "SELECT NAME,HEIGHT FROM USER WHERE NAME ='" + n + "'";
        /*SQL statment for finding user in table*/
        exit = sqlite3_exec(DB, sql.c_str(), save_callback, 0, &error_message);
        test_open(exit, "Check user successful");
    }
    close_database();
    if (strcmp(name.c_str(),"" ) == 0) {
        return false;
    }
    else return true;
}

bool Database::update_user_height(std::string n,int h)
{
    int exit = open_database();
    if (!exit) {//Database opened succsessfully
        /*Create string for sql Query*/
        std::string sql = { "UPDATE USER set HEIGHT = " + std::to_string(h) + " WHERE NAME ='" + n + "'" };
        /*SQL statment for finding user in table*/
        exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &error_message);
        test_open(exit, "Update user successfully");
    }
    close_database();
    return false;
}

int Database::return_user_height(std::string name)
{
    int exit = open_database();
    if (!exit) {//Database opened succsessfully
        /*Create string for sql Query*/
        std::string sql = "SELECT NAME,HEIGHT FROM USER WHERE NAME ='" + name + "'";
        /*SQL statment for finding user in table*/
        exit = sqlite3_exec(DB, sql.c_str(), save_callback, 0, &error_message);
        test_open(exit, "Get data from table successfull");
    }
    close_database();
    
    return std::stoi(height);

}