#include "Coms_EN.h"

void Coms::intro()
{
	std::cout <<std::endl<< "Welcome to this height measure system." << std::endl;
}

void Coms::menu_main()
{
	std::cout << "Main menu:" << std::endl
		<< "1 : New measurment" << std::endl
		<< "2 : Excisting measurments" << std::endl
		<< "3 : Settings" << std::endl
		<< "0 : Turn off system" << std::endl;
}

void Coms::menu_new()
{
	std::cout << " New measurment menu:" << std::endl
		<< "1 : Take new measurment" << std::endl
		<< "0 : Back to main menu" << std::endl;
}

void Coms::menu_excisting()
{
	std::cout << "Excisting measurments menu:" << std::endl
		<< "1 : Search for measurment" << std::endl
		<< "2 : View all database entries " << std::endl
		<< "0 : Back to main menu" << std::endl;
}

void Coms::menu_settings()
{
	std::cout << "Instillinger:" << std::endl
		<< "1 : Start setup" << std::endl
		<< "2 : Zero distance to sensor" << std::endl
		<< "3 : Delete database table" << std::endl
		<< "0 : Back to main menu" << std::endl;

}

void Coms::end()
{
	std::cout << "Thank you for the use of this system." << std::endl;
}

void Coms::error_message(std::string error)
{
	std::cout << "An error as appeared." << std::endl
			  <<"ERROR KODE: "<<error<<std::endl;
}

void Coms::wrong_input()
{
	std::cout << "You input appears invalid, please try again." << std::endl << std::endl;
}

int Coms::get_menu_int(int options)//Ask for int within range 0 <= input <= options
{
	std::string input; //what user writes in console.
	std::cout << "Choose wanted menu option: ";
	std::cin >> input; //get uset input
	std::cout << std::endl;
	for (int i = 0; i <= options; i++) { //go through and check if input matches and int in options
		if (std::to_string(i) == input) {
			return i; // Return chosen int within range 0 <= input <= options
		}
	}
	wrong_input();
	return -1;
}

void Coms::ask_for_name()
{
	std::cout << "Your name: ";
}
