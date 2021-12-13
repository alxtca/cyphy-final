#include "./libs/mongoose.h"
#include "./libs/mjson.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "inja.hpp"

#include "database.h"
#include "take-height-measurement.h"
#include "read-distance-and-update-xml.h"
#include "Coms_EN.h" // For English language
//#include "button.h"


// Just for convenience
using namespace inja;
namespace fs = std::filesystem;

Database DB("EN");

//for returns from fileFind
struct FileFindReturn {
  std::string message;
  std::string file_name;
  int above;//if I don't have picture for given height, I'll display next height person and tell "he is Xcm above you".
};
//Search for a requested file "height.jpg", if no file was found, return empty string, otherwise return file name.
std::string FileScan(std::string fl_to_find){
  std::string rslt = "";
  for (const auto & entry : fs::directory_iterator("./html/img")){
    if (entry.path().filename().string() == fl_to_find){
      return entry.path().filename().string();
    }
  }
  return rslt;
}
//search a file corresponding to a height. return message depending on height range.
FileFindReturn fileFind(unsigned int height){
  FileFindReturn m;
  std::string file_to_find = std::to_string(height) + ".jpg"; //this is a file name to search
  std::string file_was_found = "";//file_was_found will store file name if it was found

  if (height < 135) {
    m.message = "Du er lavere enn Peter Dinklage";
    m.file_name = "135.jpg";
    m.above = 135 - height; //he is that much higher than you
    return m;
  }
  else if (height > 216) {
    m.message = "Du er høyere enn Shaquille O'Neal";
    m.file_name = "215.jpg";
    m.above = height - 216; //he is that much lower than you
    return m;
  }
  else {
    file_was_found = FileScan(file_to_find);
    unsigned int height_above{height};
    //if no such file exist, search for file with higher person.
    while (file_was_found == ""){
      height_above++;
      file_to_find = std::to_string(height_above) + ".jpg";
      file_was_found = FileScan(file_to_find);
      }
    if (height_above > height) m.message = "Du er lavere enn";
    else m.message = "Du er like høy som ";
    m.file_name = file_was_found;
    m.above = height_above - height;
    return m;
  }
}


//handler function
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    json data;
    data["error"] = ""; data["title"] = "";

// --------------------  INDEX page -----------------------------------------------
    if (mg_http_match_uri(hm, "/")) {
      //fill some data for template
      std::string result = render("{% include \"html/index.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
// ------------------- NEW MEASUREMENT ---------------------------------------
    else if (mg_http_match_uri(hm, "/new_measurement")) {
      //fill some data for template
      std::string result = render("{% include \"html/new_measurement.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
//-------------------- NEW MEASUREMENT FORM -------------------------------
    else if (mg_http_match_uri(hm, "/new_measurement_form")) {
      //fill some data for template
      std::string result = render("{% include \"html/new_measurement_form.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
//---------------------CHECK USER IN DB (for register new measurement) -----------------------------------
    else if (mg_http_match_uri(hm, "/check_new_user")) {
      char buf[20] = "";
      mg_http_get_var(&hm->body, "user_name", buf, 20);
      std::cout << "extracted: " << buf << std::endl;

      //check in database //dbCheck to return false if user not found in DB
      //Database DB("EN");
      bool user_in_db = DB.username_Exist(buf); //if exist return true, if not return false
      std::cout << "Server - user exist in db: " << user_in_db << std::endl;
      if (user_in_db == false) { //if user does't exist in db
        data["u_name"] = buf;
        data["title"] = "Bruker navn er ledig.";
        std::string result2 = render("{% include \"html/take_measurement.html\" %}", data);
        mg_http_reply(c, 200, NULL, result2.c_str());
      } 
      else {//if user exist in db
      //return to new measurement form with error message
        data["error"] = "Bruker navn er opptatt, velg en annen";
        std::string result2 = render("{% include \"html/new_measurement_form.html\" %}", data);
        mg_http_reply(c, 200, NULL, result2.c_str());
      }
    }
//------------------TAKE MEASURE NOW  ------------------------
    else if (mg_http_match_uri(hm, "/reg_height")) {
      //extract user_name from html form
      char buf2[20] = "";
      mg_http_get_var(&hm->body, "user_name", buf2, 20);
      std::cout << "extracted: " << buf2 << std::endl;
      data["u_name"] = buf2;

      //take measurement (activate ultrasonic sensor)
        //read height from txt file
/*
      std::ifstream MyReadFile("data.txt");//temp
      std::string temp_height;//temp
      std::getline (MyReadFile, temp_height);//temp
      int height = std::stoi(temp_height);//temp
*/
      int height = takeHeightMeasurement();
      data["height"] = height;

      FileFindReturn message_to_render = fileFind(height);
      data["message"] =  message_to_render.message;
      data["file_name"] =  message_to_render.file_name;
      data["above"] =  message_to_render.above;

      //render template with that user_name and height
      std::string result3 = render("{% include \"html/result.html\" %}", data);
      mg_http_reply(c, 200, NULL, result3.c_str());

      //store values to DB
      //Database DB("EN");
    
      if (DB.username_Exist(buf2) == false){ //hvis finnes ikke lag ny bruker
        DB.write_user(buf2, height);
        DB.get_last_user();
        DB.print_to_lcd();
      }
      else { //ellers oppdater eksisterende
        DB.update_user_height(buf2, height);
      }

    }
//-------------------- UPDATE EXISTING USER HEIGHT ------------------
    else if (mg_http_match_uri(hm, "/update_existing_user_form")) {
      //fill some data for template
      std::string result = render("{% include \"html/update_existing_user_form.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
//---------------------CHECK EXISTING USER IN DB (TO UPDATE HEIGHT) -----------------------------------
    else if (mg_http_match_uri(hm, "/check_existing_user")) {
      char buf[20] = "";
      mg_http_get_var(&hm->body, "user_name", buf, 20);
      std::cout << "extracted: " << buf << std::endl;

      //check in database //dbCheck to return false if user not found in DB
      //Database DB("EN");
      bool user_in_db = DB.username_Exist(buf);

      //bool user_in_db = false; //dummy for testing

      if (user_in_db == true) { //if user exist in db
        data["u_name"] = buf;
        data["title"] = "Klar å oppdatere høyde";
        std::string result2 = render("{% include \"html/take_new_measurement.html\" %}", data);
        mg_http_reply(c, 200, NULL, result2.c_str());
      } 
      else {//if user do not exist in db
      //return to new measurement form with error message
        data["error"] = "Bruker navn finnes ikke i database";
        std::string result2 = render("{% include \"html/update_existing_user_form.html\" %}", data);
        mg_http_reply(c, 200, NULL, result2.c_str());
      }
    }
//-------------------- EXISTING MEASUREMENT FORM------------------
    else if (mg_http_match_uri(hm, "/existing_measurement_form")) {
      //fill some data for template
      std::string result = render("{% include \"html/existing_measurement_form.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
//---------------------GET USER DATA FROM DATABASE -----------------------------------
    else if (mg_http_match_uri(hm, "/get_user_data")) {
      char buf[20] = "";
      mg_http_get_var(&hm->body, "user_name", buf, 20);
      std::cout << "extracted: " << buf << std::endl;

      //check in database //dbCheck to return false if user not found in DB
      //bool user_in_db = false; // dummy for test
      //Database DB("EN");
      bool user_in_db = DB.username_Exist(buf);


      if (user_in_db == true) { //if user exist in db
          //read height from txt file
 /*
        std::ifstream MyReadFile("data.txt");
        std::string temp_height;
        std::getline (MyReadFile, temp_height);
        int height = std::stoi(temp_height);
*/
        int height = DB.return_user_height(buf);
        //check db return here
        data["height"] = height;

        FileFindReturn message_to_render = fileFind(height);
        data["message"] =  message_to_render.message;
        data["file_name"] =  message_to_render.file_name;
        data["above"] =  message_to_render.above;

        data["u_name"] = buf;
        std::string result2 = render("{% include \"html/result.html\" %}", data);
        mg_http_reply(c, 200, NULL, result2.c_str());
      } 
      else {//if user do not exist in db
      //return to form with error message
        data["error"] = "Bruker navn finnes ikke i database";
        std::string result2 = render("{% include \"html/existing_measurement_form.html\" %}", data);
        mg_http_reply(c, 200, NULL, result2.c_str());
      }
    }
//-------------------- SETTINGS ------------------
    else if (mg_http_match_uri(hm, "/settings")) {    
      std::string result = render("{% include \"html/settings.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
//-------------------- SET BASELINE------------------
    else if (mg_http_match_uri(hm, "/set_baseline")) {
      int baseline = readDistanceAndUpdateXml();
      //int baseline = 230; //dummy for testing
      data["baseline"] = baseline;

      std::string result = render("{% include \"html/display_new_baseline.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }
//-------------------- START SETUP ------------------
    else if (mg_http_match_uri(hm, "/start_setup")) {
      // run setup()  ///////////REPLACE here

      std::string result = render("{% include \"html/setup_results.html\" %}", data);
      mg_http_reply(c, 200, NULL, result.c_str());
    }

    //add rendering here /// or error 404 handler
    else {
      struct mg_http_serve_opts opts = {.root_dir = "./html"};
      mg_http_serve_dir(c, hm, &opts);
    }
  }
  (void) fn_data;
}

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);                                        // Init manager

  DB.create_table(); 

  mg_http_listen(&mgr, "http://localhost:8000", fn, &mgr);  // Setup listener
  for (;;) mg_mgr_poll(&mgr, 1000);                         // Event loop
  mg_mgr_free(&mgr);                                        // Cleanup
  return 0;
}

//g++-10.1 -std=c++17 -Wall -pedantic main-dynamic-min.cpp ./libs/mongoose.c ./libs/mjson.c -o main-test

//compile on windows
//g++ -o main-dynamic.exe ./libs/mongoose.c ./libs/mjson.c -lwsock32 main-dynamic.cpp -std=c++17
//g++ -Wall -g -o RP2-dynamic main-dynamic.cpp database.h database.cpp Coms_EN.h Coms_EN.cpp read-distance-and-update-xml.h read-distance-and-update_xml.cpp send-distance-request.h send-distance-request.cpp take-height-measurement.h take-height-measurement.cpp tinyxml2.h lcd1602.c lcd1602.h ./libs/mongoose.c ./libs/mjson.c tinyxml2.cpp -lsqlite3 --std=c++17 -lstdc++fs