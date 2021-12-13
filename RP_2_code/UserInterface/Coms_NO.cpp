#include "Coms_NO.h"

void Coms::intro()
{
	std::cout <<std::endl<< "Velkommen til høydemåler system." << std::endl;
}

void Coms::menu_main()
{
	std::cout << "Velg den tjenesten du har lyst å burke:" << std::endl
		<< "1 : Ny Måling" << std::endl
		<< "2 : Eksiterende Måling" << std::endl
		<< "3 : Settings" << std::endl
		<< "0 : Avslutt systemet" << std::endl;
}

void Coms::menu_new()
{
	std::cout << "Ny måling:" << std::endl
		<< "1 : Skap ny måling" << std::endl
		<< "0 : Tilbake" << std::endl;
}

void Coms::menu_excisting()
{
	std::cout << "Eksterende måling:" << std::endl
		<< "1 : Søk etter måling" << std::endl
		<< "2 : Se alle målinger" << std::endl
		<< "0 : Tilbake" << std::endl;
}

void Coms::menu_settings()
{
	std::cout << "Instillinger:" << std::endl
		<< "1 : Start Oppsett" << std::endl
		<< "2 : Nullstill høyde måler" << std::endl
		<< "3 : Slett database tabell" << std::endl
		<< "0 : Tilbake" << std::endl;
}

void Coms::end()
{
	std::cout << "Takk for bruken av høydemåler system ." << std::endl;
}

void Coms::error_message(std::string error)
{
	std::cout << "Det har ser ut til at en feil har dukket opp." << std::endl
			  <<"ERROR KODE: "<<error<<std::endl;
}

void Coms::wrong_input()
{
	std::cout << "Det har ser ut skrever inn ugyldig input, prøv igjenn." << std::endl << std::endl;
}

int Coms::get_menu_int(int options)//Ask for int within range 0 <= input <= options
{
	std::string input; //what user writes in console.
	std::cout << "Skriv inn verdi for ønsket handling: ";
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
	std::cout << "Ditt navn: ";
}
