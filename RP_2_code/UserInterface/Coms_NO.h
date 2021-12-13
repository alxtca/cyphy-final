#include <iostream>
#include <string>

class Coms //Communication functions for input/output
{
public:
	/*Intro message when opening program*/
	void intro();
	/*Main menu options explenation*/
	void menu_main();
	/*Menu for new user */
	void menu_new(); 
	/*Menu for getting exicting user data*/
	void menu_excisting();
	/*Menu for system settings*/
	void menu_settings();
	/*Goodbye message*/
	void end();
	/*Message when something goes wrong*/
	void error_message(std::string error = "UNKNOWN");
	/*when input isn't av wanted type, error message*/
	void wrong_input();
	/*Returns input  if 0 <= input <= options, else wrong_input()*/
	int get_menu_int(int options);
	/*Print request for name*/
	void ask_for_name();
};

