#include "database.h"
#include "LCD1602/lcd1602.h"




int Database::callback(void* NotUsed, int num_results, char** values, char** columns)
{
    int i;
    for (i = 0; i < num_results; i++) {
        printf("%s = %s\n", columns[i], values[i] ? values[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int Database::save_callback(void* NotUsed, int num_results, char** values, char** columns)
{
    std::string n; // Name
    std::string h; // Height
    if (num_results == 2) {
        if (language == 'EN') {
            n = "User  : ";
            h = "Height: ";
        }
        else if (language == 'NO') {
            n = "Bruker: ";
            h = "H�yde : ";

        }
        else {
            std::cout << "ERROR: database/save_callback/language" << std::endl;
        }

        n.append(values[0]);    // eks: "Bruker: username"
        h.append(values[1]);    // eks: "H�yde : 180"

        h.append(" cm");        // eks: "H�yde : 180 cm"

        //------test values--------
        std::cout << std::endl << n << std::endl << h << std::endl << std::endl;
        //------test values--------
        
        //------Write to display------
        int i2c_bus;
        i2c_bus = lcd1602Init(1,0x27); // parameters: channel, address 
        
        if (i2c_bus)
        {
            printf("Initialization failed; aborting...\n");
            return 0;
        }
    
        //Show name: 
        lcd1602SetCursor(0,0); // default position, character 0, line 0
        char lineToPrint[n.size() + 1]{};   // prepare a print array
        n.copy(lineToPrint,n.size() + 1);   // copy name-string to print array
        lineToPrint[n.size()] = '\0';       // adding stop character
        lcd1602WriteString(lineToPrint);    // write to top line
        
        //Show height:
        lcd1602SetCursor(0,1); // change line position, character 0, line 1
        lineToPrint[h.size() + 1]{};     // reset print array
        n.copy(lineToPrint,h.size() + 1);   // copy height-string to print array
        lineToPrint[h.size()] = '\0';       // adding stop character
        lcd1602WriteString(lineToPrint);    // write to bottom line    
        
            //------Print to display------

    }
    else
    {
        std::cout << "ERROR save_callback"<<std::endl;
    }

    return 0;
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


void Database::get_user(void)
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
