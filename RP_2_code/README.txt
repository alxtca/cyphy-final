### USEFEUL info

When including <sqlite3.h> 

for windows visual studio 19, do the following:

download the amalgmation files but also the dll files 
use https://sqlite.org/2021/sqlite-dll-win32-x86-3360000.zip, 
which gives a .dll and a .def file. 
Copy them to your project directory and then open up a 
Visual Studio command prompt (terminal in the View menu). 
Browse to the directory that you've copied the files to and 
type LIB /DEF:sqlite3.def. 
This will create a library file for VS to use. 
Add this file to your project dependencies at 
Project Properties -> Configuration Properties -> Linker -> Input -> Additional Dependencies 
(you'll have to type it in manually as you can't browse). 
Compiling in Visual Studio should now work successfully.

For linux
sudo apt-get install sqlite3

in userinterface file path
bash: g++ -Wall -g -o RP2 Main.cpp database.h database.cpp Coms.h Coms.cpp read-distance-and-update-xml.h read-distance-and-update_xml.cpp send-distance-request.h send-distance-request.cpp take-height-measurement.h take-height-measurement.cpp tinyxml2.h tinyxml2.cpp -lsqlite3
A file RP2 will then appear in that file path.
Double click file --> Execute in Terminal
You can now navigate the console for RP2

For (live system) / (local test on windows)
Go into the following files:

database.h, follow instructions at topp of file.

take-height-measurment.cpp and within int takeHeightMeasurement(), follow instructions on what lines to add //

send-distance-request.h, follow instructions at topp of file.

send-distance-request.cpp, follow instructions at topp of file.
