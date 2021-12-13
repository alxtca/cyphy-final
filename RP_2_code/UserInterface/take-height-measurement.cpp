#include "send-distance-request.h"

#include "tinyxml2.h"

#define XMLDOC "data.xml"
#define SENSORTAG "sensor"

//calculate height ( based on data.xml )
int calculateHeight(int & d);
int getTagValue(const char * tag);

int takeHeightMeasurement() {

    //request distance from RP1
    char request_message[] = "height";
    int distance = sendRequest(request_message); // For Live system, // line below
    //int distance = 100; // For local test, // line above
    return calculateHeight(distance);
}

int calculateHeight(int & distance) {
    //get sensor position from data.xml
    int sensor_position_height{getTagValue((char*)(SENSORTAG))};

    return (sensor_position_height - distance);
}

//to get Int value store in the tag "sensor"
int getTagValue(const char * tag){
  int sensor_position;
  tinyxml2::XMLDocument xml_doc;

  tinyxml2::XMLError eResult = xml_doc.LoadFile(XMLDOC);
  if (eResult != tinyxml2::XML_SUCCESS) return false;

  tinyxml2::XMLNode* root = xml_doc.FirstChildElement("root");
  if (root == nullptr) return false;

  tinyxml2::XMLElement* element = root->FirstChildElement(tag);
  if (element == nullptr) return false;

  // access data inside the element. Must know data type you trying to access.
  eResult = element -> QueryIntText(&sensor_position);

  return sensor_position;
}