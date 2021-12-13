#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string>
#include "LCD1602/lcd1602.h"

//
// LCD1602 2 line by 16 character LCD demo
//
// Written by Larry Bank - 12/7/2017
// Copyright (c) 2017 BitBank Software, Inc.
// bitbank@pobox.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


//int main(int argc, char *argv[])
int main()
{
int rc;
	rc = lcd1602Init(1, 0x27);
	if (rc)
	{
		printf("Initialization failed; aborting...\n");
		return 0;
	}
	
	std::string a_string{"BitBank LCD1602"};
	char lineToPrint[a_string.size() + 1]{};
	a_string.copy(lineToPrint,a_string.size() + 1);
	lineToPrint[a_string.size()] = '\0';
	lcd1602WriteString(lineToPrint);
	
	lcd1602SetCursor(0,1);
	std::string another_string{"Hello"};
	lineToPrint[another_string.size() + 1];
	another_string.copy(lineToPrint, another_string.size() + 1);
	lineToPrint[another_string.size()] = '\0';	
	lcd1602WriteString(lineToPrint);
	//lcd1602SetCursor(0,1);
	//char exit[]{"ENTER to exit"};
	//lcd1602WriteString(exit);
	lcd1602Control(1,0,1); // backlight on, underline off, blink block on 
	getchar();
	lcd1602Shutdown();
return 0;
} /* main() */
