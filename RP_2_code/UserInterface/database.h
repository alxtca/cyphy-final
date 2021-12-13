#include <iostream>
#include <vector>
#include <ctime>
//#include <string> // For windows
#include <string.h> for Linux
//#include "sqlite3/sqlite3.h"  //For windows, // line below
# include <sqlite3.h> //For Linux, // line above
#include "lcd1602.h" // lcd library


class Database
{

	private: 
		/*Name of the database*/
		std::string database_name ="HeightMeasure.db";
		/*Variable for Language*/
		char* language;
		/*Pointer to the database*/
		sqlite3 *DB;
		/*Error message for SQL errors*/
		char *error_message = 0;
		/*pointer to this class/db*/
		static Database* p_this_db;
		/*Temp values to hold data for lcd*/
		std::string p_name, p_height;
		/*Temp user data*/
		std::string name, height;
		/*Used in sqlite3_exec*/
		static int callback(void* NotUsed, int num_results, char** values, char** columns);
		/*Used in sqlite3_exec(SELECT) for saving retuned values */
		static int save_callback(void* NotUsed, int num_results, char** values, char** columns);
		/*Send temp data to lcd*/
		void send_to_lcd();
		/*Opens the database*/
		int open_database();
		/*Check if database interaction is succsessfull*/
		void test_open(int exit, std::string success_message);


	public:
		Database(char* chosen_language) {
			p_this_db = this;
			p_this_db->language = chosen_language;
		}

		/*Create a table in database*/
		void create_table();
		/*Insert a new user into database*/
		void write_user(std::string name, int height);
		/*Finds if User with name (Name), if so, prints*/
		void read_user(std::string name);
		/*Finds lased saved user and saves data locally*/
		void get_last_user(void);
		/*Print to lcd*/
		void print_to_lcd();
		/*Closes the database*/
		void close_database();
		/*Se all of database content*/
		void view_database();
		/*Deletes the table in database*/
		void clear_database();
		/*Return true if user name in use*/
		bool used_user_name(std::string name);
		/*Update height h user with name name*/
		bool update_user_height(std::string name,int h);
		/*Return height of user with name name*/
		int return_user_height(std::string name);
		static int callback_username_Exist(void* NotUsed, int args, char** argv, char** azColName);
		bool username_Exist(std::string user_n);
		bool user_exist;

};