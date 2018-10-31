#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
using namespace std;

struct client_str
{
    string client_alias;
    string client_ip;
    int client_port;
    string server_ip;
    int server_port;
    int downloading_port;
    string client_root;
} clt;

int main(int argc, char *argv[])
{
    if(argc!=8)
    {
        printf("not according to format \n");
        printf("try to connect again..!! \n");
        exit(1);
    }
    else
    {
        clt.client_alias    =   argv[1];
        clt.client_ip       =   argv[2];
        clt.client_port     =   atoi(argv[3]);
        clt.server_ip       =   argv[4];
        clt.server_port     =   atoi(argv[5]);
        clt.downloading_port=   atoi(argv[6]);
        clt.client_root     =   argv[7];
    }




    int client;
    //int port_no = 1500; // NOTE that the port number is same for both client and server
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    // string ip = "127.0.0.1";

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);


    cout << "\n Socket client has been created..." << endl;



    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(clt.server_port);


    inet_pton(AF_INET, clt.server_ip.c_str(), &server_address.sin_addr);


    if (connect(client,(struct sockaddr *)&server_address, sizeof(server_address)) == 0)
        cout << "Connection to the server port number: " << clt.server_port << endl;

    char y[10];
    cout << "Connection confirmed, you are good to go...";
    
    string can=clt.client_alias;
    //string clientfile=clt.client_alias+":"+clt.client_ip+":"+clt.client_port+":"+clt.downloading_port;
    cout << "\n\n=> Enter & to end the connection\n" << endl;
    do
    {   //send(client,clientfile.c_str,bufsize,0);
        //recv(client,y,bufsize,0);
        send(client, can.c_str(), bufsize, 0);
        recv(client,y, bufsize, 0);
        //sending client alias name
        cout << "Client: ";
        cin.getline(buffer,bufsize);
        send(client, buffer, bufsize, 0);
        if (*buffer == '&')
        {
            send(client, buffer, bufsize, 0);
            isExit = true;
        }


        recv(client, buffer, bufsize, 0);
        cout << buffer << " ";
        if (*buffer == '&')
        {
            isExit = true;
        }
        cout << endl;
    }
    while (!isExit);
    cout << "\n=> Connection terminated.\nGoodbye...\n";

    close(client);
    return 0;
}
