#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define MSG_LEN 1024
#define USER_NOT_FOUND -1
#define NO_RECOMMENDATION -2
#define COUNTRY_NOT_FOUND -3
using namespace std;

int main() {
    cout << "Client 1 is up and running" << endl;


    struct addrinfo hints;
    struct addrinfo *server_addrinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int get_status;
    if ((get_status = getaddrinfo(NULL, "33319", &hints, &server_addrinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(get_status));
        exit(1);
    }

    int network_socket;
    if ((network_socket = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo-> ai_protocol)) == -1) {
        printf("socket error\n");
        exit(1);
    }

    int connection_status;
    if ((connection_status = connect(network_socket, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen)) == -1) {
        printf("connection error\n");
        exit(1);
    }


    while(1) {    
        cout << "Please enter the User ID: ";
        int user_id;
        cin >> user_id;
        string user_id_str = to_string(user_id);

        cout << "Please enter the Country Name: ";
        string area_name_str;
        cin >> area_name_str;

        string client_message_str;
        client_message_str.append(area_name_str);
        client_message_str.append(" ");
        client_message_str.append(user_id_str);
        send(network_socket, client_message_str.c_str(), MSG_LEN, 0);
        cout << "Client xxx has sent User " << user_id << " and " << area_name_str << " to Main Server using TCP" << endl;

        char server_response[MSG_LEN];
        recv(network_socket, &server_response, MSG_LEN, 0);

        string recommended_user_str = server_response;
        istringstream iss;
        iss.str(recommended_user_str);
        long recommended_user_id;
        iss >> recommended_user_id;

        if (recommended_user_id == NO_RECOMMENDATION) {
            cout << "You have already connect to everyone in this area." << endl;
        }
        else if (recommended_user_id == COUNTRY_NOT_FOUND) { cout << area_name_str << " not found" << endl; }
        else if (recommended_user_id == USER_NOT_FOUND) { cout << "User " << user_id << " not found" << endl; }
        else { cout << "Client xxx has received results from Main Server: User " 
                    << recommended_user_id 
                    << " is possible friend of User "
                    << user_id 
                    << " in " 
                    << area_name_str 
                    << endl; }
    }

    close(network_socket);

    return 0;
}
