//this program is a setting option in main menu
//it shall call send-distance-request()
//and save returned value to a file data.xml

/* to be called from main menu like this:

#include "read-height-and-update-xml.h"

int main()
{
    readDistanceAndUpdateXml();
}

*/

#include <iostream>
#include <fstream>
#include <string>
#include "send-distance-request.h"
#include "tinyxml2.h"

#define XMLDOC "data.xml"
#define SENSORTAG "sensor"

int storeHeightInXML(int new_sensor_position){
	tinyxml2::XMLDocument xml_doc; //create xml document
  tinyxml2::XMLError eResult = xml_doc.LoadFile(XMLDOC);
  if (eResult != tinyxml2::XML_SUCCESS) return false;

  tinyxml2::XMLNode* root = xml_doc.FirstChildElement("root");
  if (root == nullptr) return false;

  tinyxml2::XMLElement* element = root->FirstChildElement(SENSORTAG);
  if (element == nullptr) return false;

  //set new value into tag "sensor"
  element->SetText(new_sensor_position);
  xml_doc.SaveFile(XMLDOC);

  return true;
}

int readDistanceAndUpdateXml() {
    //request distance
    char request_message[] = "height";
    int distance = sendRequest(request_message);

    if (distance == 10000) {
        std::cout << "bad request" << std::endl;
        return 0;
    }
    else {
        std::cout << "Height to sensor is: " << distance << std::endl;
    }
    //store sensor position in tag "sensor" in file data.txt 
    if (storeHeightInXML(distance)){
        std::cout << "Height to sensor saved: " << distance << std::endl;
    } else {
        std::cout << "something went wrong " << std::endl;
    }

    return distance;
}