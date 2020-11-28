#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#define BACKLOG 10
#define MSG_LEN 1024
#define USER_NOT_FOUND -1
#define NO_RECOMMENDATION -2
#define COUNTRY_NOT_FOUND -3
using namespace std;

int main() {
    cout << "The Main server is up and running." << endl;


    // -----------------------------------------------------------------------------------------------------
    // connect with backserverA and B
    struct addrinfo hints_back;
    struct addrinfo *server_addrinfo_back;

    memset(&hints_back, 0, sizeof hints_back);
    hints_back.ai_family = AF_INET;
    hints_back.ai_socktype = SOCK_DGRAM;
    hints_back.ai_flags = AI_PASSIVE;

    int get_status;
    if ((get_status = getaddrinfo(NULL, "32319", &hints_back, &server_addrinfo_back)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(get_status));
        exit(1);
    }

    int server_socket_to_back;
    if ((server_socket_to_back = socket(server_addrinfo_back->ai_family, server_addrinfo_back->ai_socktype, server_addrinfo_back->ai_protocol)) == -1) {
        printf("socket error\n");
        exit(1);
    }

    int bind_status;
    if ((bind_status = bind(server_socket_to_back, server_addrinfo_back->ai_addr, server_addrinfo_back->ai_addrlen)) != 0) {
        printf("bind error\n");
        exit(1);
    }

    string message_to_back_str = "Now send the countries you have to me.\n";
    const char *message_to_back = message_to_back_str.c_str();

    struct sockaddr_in sockaddr_A;
    socklen_t addr_size_A = sizeof sockaddr_A;
    sockaddr_A.sin_family = AF_INET;
    sockaddr_A.sin_port = htons(30319);
    sockaddr_A.sin_addr.s_addr = INADDR_ANY;

    int sent_bytes_back;
    if ((sent_bytes_back = sendto(server_socket_to_back, message_to_back, MSG_LEN, 0, (struct sockaddr *) &sockaddr_A, addr_size_A)) <= 0) {
        printf("send error\n");
        exit(1);
    }

    struct sockaddr_in sockaddr_B;
    socklen_t addr_size_B = sizeof sockaddr_B;
    sockaddr_B.sin_family = AF_INET;
    sockaddr_B.sin_port = htons(31319);
    sockaddr_B.sin_addr.s_addr = INADDR_ANY;

    if ((sent_bytes_back = sendto(server_socket_to_back, message_to_back, MSG_LEN, 0, (struct sockaddr *) &sockaddr_B, addr_size_B)) <= 0) {
        printf("send error\n");
        exit(1);
    }

    struct sockaddr_in remote_addr_A;
    socklen_t addr_size_remote_A = sizeof remote_addr_A;
    char area_message_A[MSG_LEN];
    int bytes_receiving_back;
    if ((bytes_receiving_back = recvfrom(server_socket_to_back, &area_message_A, MSG_LEN, 0, (struct sockaddr *) &remote_addr_A, &addr_size_remote_A)) <= 0) {
        printf("Not receiving message from backend server A.\n");
        exit(1);
    }
    cout << "The Main server has received the country list from server A using UDP over port 31319" << endl;

    struct sockaddr_in remote_addr_B;
    socklen_t addr_size_remote_B = sizeof remote_addr_B;
    char area_message_B[MSG_LEN];
    if ((bytes_receiving_back = recvfrom(server_socket_to_back, &area_message_B, MSG_LEN, 0, (struct sockaddr *) &remote_addr_B, &addr_size_remote_B)) <= 0) {
        printf("Not receiving message from backend server B.\n");
        exit(1);
    }
    cout << "The Main server has received the country list from server B using UDP over port 31319" << endl;

    string area_message_A_str = area_message_A;
    string area_message_B_str = area_message_B;
    istringstream iss(area_message_A_str);
    string area_name;
    vector<string> area_names_A;
    while (iss >> area_name) { area_names_A.push_back(area_name); }
    iss.str("");
    iss.clear();

    iss.str(area_message_B_str);
    vector<string> area_names_B;
    while (iss >> area_name) { area_names_B.push_back(area_name); }


    // Prints the on screen message
    int min_num_lines = min(area_names_A.size(), area_names_B.size());
    int max_num_lines = max(area_names_A.size(), area_names_B.size());
    int max_str_len = -1;
    for (int i = 0; i < area_names_A.size(); i++) { max_str_len = max(max_str_len, (int)area_names_A[i].length()); }
    max_str_len = max(max_str_len, 8);
    ostringstream oss;
    oss << "Server A";
    for (int i = 0; i < max_str_len - 8; i++) { oss << " "; }
    oss << "|Server B" << endl;
    for (int i = 0; i < min_num_lines; i++) {
        oss << area_names_A[i];
        for (int j = 0 ; j < max_str_len - (int)area_names_A[i].length(); j++) { oss << " "; }
        oss << "|" << area_names_B[i] << endl;
    }
    if (area_names_A.size() < area_names_B.size()) {
        for (int i = 0; i < max_num_lines - min_num_lines; i++) {
            oss << "        |" << area_names_B[i + min_num_lines] << endl;
        }
    }
    else if (area_names_A.size() > area_names_B.size()) {
        for (int i = 0; i < max_num_lines - min_num_lines; i++) {
            oss << area_names_A[i + min_num_lines];
            for (int j = 0; j < max_str_len - (int)area_names_A[i + min_num_lines].length(); j++) { oss << " "; }
            oss << "|" << endl;
        }
    }
    string country_message(oss.str());
    oss.str("");
    oss.clear();
    cout << country_message;




    // -----------------------------------------------------------------------------------------------------
    // connect with client
    struct addrinfo hints;
    struct addrinfo *server_addrinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "33319", &hints, &server_addrinfo);

    int server_socket_to_client;
    server_socket_to_client = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo->ai_protocol);

    int yes = 1;
    setsockopt(server_socket_to_client, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

    bind(server_socket_to_client, server_addrinfo->ai_addr, server_addrinfo->ai_addrlen);

    listen(server_socket_to_client, BACKLOG);

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;

    int bytes_receiving;
    char client_message[MSG_LEN];
    string client_message_str;
    string user_area_name;
    int user_id;
    char recommendation[MSG_LEN];

    string no_user_error_str = to_string(USER_NOT_FOUND);
    char *no_user_error = &no_user_error_str[0];
    string no_recom_str = to_string(NO_RECOMMENDATION);
    char *no_recom = &no_recom_str[0];
    string no_country_error_str = to_string(COUNTRY_NOT_FOUND);
    char *no_country_error = &no_country_error_str[0];
    string recom_str;
    char *recom;

    pid_t childpid;

    while(1) {
        int child_socket = accept(server_socket_to_client, (struct sockaddr *) &client_addr, &addr_size);
        if (child_socket < 0) { exit(1); }

        if ((childpid = fork()) == 0) { 
            close(server_socket_to_client); 
            while(1) {
                recv(child_socket, &client_message, MSG_LEN, 0);
                client_message_str = client_message;
                istringstream iss(client_message_str);
                iss >> user_area_name;
                iss >> user_id;
                iss.str("");
                iss.clear();
                cout << "The Main server has received the request on User " << user_id << " in " << user_area_name << " from client 1 using TCP over port 33319" << endl; 

                // -----------------------------------------------------------------------------------------------------
                // Determine which server to call
                if ((find(area_names_A.begin(), area_names_A.end(), user_area_name) != area_names_A.end()) || (find(area_names_B.begin(), area_names_B.end(), user_area_name) != area_names_B.end())) {
                    if (find(area_names_A.begin(), area_names_A.end(), user_area_name) != area_names_A.end()) {
                        cout << user_area_name << " shows up in server A" << endl;
                        // Send user information to A
                        sendto(server_socket_to_back, client_message, MSG_LEN, 0, (struct sockaddr *) &sockaddr_A, addr_size_B);
                        cout << "The Main Server has sent request from User " << user_id << " to server A using UDP over port 32319" << endl;
                        // Receive recommendation from A
                        recvfrom(server_socket_to_back, &recommendation, MSG_LEN, 0, (struct sockaddr *) &remote_addr_A, &addr_size_remote_A);
                        cout << "The Main server has received searching result(s) of User " << user_id << " from server A" << endl;
                    }
                    else {
                        cout << user_area_name << " shows up in server B" << endl;
                        // Send user information to B
                        sendto(server_socket_to_back, client_message, MSG_LEN, 0, (struct sockaddr *) &sockaddr_B, addr_size_B);
                        cout << "The Main Server has sent request from User " << user_id << " to server B using UDP over port 32319" << endl;
                        // Receive recommendation from B
                        recvfrom(server_socket_to_back, &recommendation, MSG_LEN, 0, (struct sockaddr *) &remote_addr_B, &addr_size_remote_A);
                        cout << "The Main server has received searching result(s) of User " << user_id << " from server B" << endl;
                    }
                    // Send recommendation back to user
                    string recommendation_str = recommendation;
                    //cout << "From back: " << recommendation_str << endl;
                    iss.str(recommendation_str);
                    string responding_server;
                    iss >> responding_server;
                    long recom_user;
                    iss >> recom_user;
                    iss.str("");
                    iss.clear();
                    if (recom_user == USER_NOT_FOUND) { 
                        cout << "The Main server has received \"User ID: Not found\" from server " << responding_server << endl;
                        send(child_socket, no_user_error, MSG_LEN, 0);
                        cout << "The Main Server has sent error to client using TCP over port 33319" << endl;
                    }
                    else if (recom_user == NO_RECOMMENDATION) {
                        cout << "The Main server has received \"No more recommendation\" from server " << responding_server << endl;
                        send(child_socket, no_recom, MSG_LEN, 0);
                        cout << "The Main Server has sent message to client using TCP over port 33319" << endl;
                    }
                    else {
                        recom_str = to_string(recom_user);
                        recom = &recom_str[0];
                        //cout << "To client: " << recom << endl;
                        send(child_socket, recom, MSG_LEN, 0);
                        cout << "The Main Server has sent searching result to client using TCP over port 33319" << endl;
                    }
                }
                else {
                    cout << user_area_name << " does not show up in server A&B" << endl;
                    send(child_socket, no_country_error, MSG_LEN, 0);
                    cout << "The Main Server has sent \"Country Name: Not found\" to client 1 using TCP over port 33319" << endl;
                }
            memset(client_message, 0, MSG_LEN);
            client_message_str.clear();
            iss.str("");
            iss.clear();
            user_area_name.clear();
            memset(recommendation, 0, MSG_LEN);
            }
        }
    }


    close(server_socket_to_back);
    freeaddrinfo(server_addrinfo_back);


    close(server_socket_to_client);
    freeaddrinfo(server_addrinfo);
    return 0;
}