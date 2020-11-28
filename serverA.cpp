#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "readfile.h"
#include "recommend.h"
#define MSG_LEN 1024
#define USER_NOT_FOUND -1
#define NO_RECOMMENDATION -2
using namespace std;

int main() {
   cout << "The server A is up";



   // Read data from file
   vector<vector<vector<unsigned int>>> all_areas_table = read_all_users("data1.txt");
   vector<string> all_areas_name = read_all_areas("data1.txt");

   ostringstream oss;
   for (int i = 0; i < all_areas_table.size(); i++) {
      oss << all_areas_name[i] << " ";
   }
   string all_areas_name_str(oss.str());
   oss.str("");
   oss.clear();
   const char *all_areas_name_arr = all_areas_name_str.c_str();



   // Communicate with main server
   struct addrinfo hints;
   struct addrinfo *server_addrinfo;

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_DGRAM;
   hints.ai_flags = AI_PASSIVE;

   getaddrinfo(NULL, "30319", &hints, &server_addrinfo);

   int server_socket_to_main;
   server_socket_to_main = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo->ai_protocol);

   bind(server_socket_to_main, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen);
   cout << " and running using UDP on port <30319>" << endl;

   struct sockaddr_in sockaddr_main;
   socklen_t addr_size = sizeof sockaddr_main;



   // Receive bootup message from main server
   char bootup_message[MSG_LEN];
   recvfrom(server_socket_to_main, &bootup_message, MSG_LEN, 0, (struct sockaddr *) &sockaddr_main, &addr_size);



   // Send area names to main server
   sendto(server_socket_to_main, all_areas_name_arr, MSG_LEN, 0, (struct sockaddr *) &sockaddr_main, addr_size);
   cout << "The server A has sent a country list to Main Server" << endl;


   char main_message[MSG_LEN];
   string main_message_str;
   istringstream iss;
   string area_name;
   int user_id;
   long recommended_user;
   char *recommendation;


   while(1) {

      // Receive area name and user id from main server
      recvfrom(server_socket_to_main, &main_message, MSG_LEN, 0, (struct sockaddr *) &sockaddr_main, &addr_size);


      // Decode the message
      main_message_str = main_message;
      iss.str(main_message_str);
      iss >> area_name;
      iss >> user_id;
      iss.str("");
      iss.clear();
      cout << "The server A has received request for finding possible friends of User " << user_id << " in " << area_name << endl;

      recommended_user = recommend(all_areas_table, all_areas_name, area_name, user_id, "A");
      oss << "A " << recommended_user;
      string recommendation_str(oss.str());
      oss.str("");
      oss.clear();
      recommendation = &recommendation_str[0];



      // Send recommendation to main server
      sendto(server_socket_to_main, recommendation, MSG_LEN, 0, (struct sockaddr *) &sockaddr_main, addr_size);
      cout << "The server A has sent the result(s) to Main Server" << endl;

      recommendation_str.clear();
   }



   close(server_socket_to_main);
   freeaddrinfo(server_addrinfo);
   return 0;
}
