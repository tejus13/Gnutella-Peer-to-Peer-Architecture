#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <locale> 
#include <fstream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;
struct repository
{
    string file_name;
    string path_name;
    string alias_name;

} repos;
struct client_info
{
    string alias_name2;
    string client_ip;
    string client_port;
    string downloading_port;
} cinfo;

vector<client_info> c;
vector<client_info> c1;
vector<repository> v;
vector<repository> v1;

void *thread_create(void *);

struct server_str
{
    int server_port;
    string repo_file;
    string server_ip;
    string client_list_file;
    string server_root;

} crs;
int main(int argc, char *argv[])
{

    if(argc!=6)
    {
        printf("not according to format \n");
        printf("try to connect again..!! \n");
        exit(1);
    }
    else
    {
        crs.server_ip           =   argv[1];
        crs.server_port         = atoi(argv[2]);
        crs.repo_file           =   argv[3];
        crs.client_list_file    =   argv[4];
        crs.server_root         =   argv[5];
    }

    /*==================ds for repo file==================*/
    /*====================================================*/
    int client;
    string data;
    fstream  afile;
    int len;
    afile.open(crs.repo_file, ios::out | ios::in);
    while(getline(afile,data))
    {
        len=data.length();
        int strt=0;
        int flag=0;
        for(int i=strt; i<len; i++)
        {
            if(data[i]==':'&&flag==0)
            {
                repos.file_name=data.substr(0,i);
                strt=i+1;
                flag=1;
                continue;
            }

            if(data[i]==':'&&flag==1)
            {
                repos.path_name=data.substr(strt,i-strt);
                strt=i+1;
                continue;
            }

            repos.alias_name=data.substr(strt,len-strt);

        }
        v.push_back(repos);
    }


    /*===================== ds for client file======================*/
    /*==============================================================*/
    fstream afile2;
    int lenth;
    string data2;
    afile2.open(crs.client_list_file, ios::out | ios::in);
    while(getline(afile2,data2))
    {
        lenth=data2.length();
        int strt=0;
        int flag=0;
        for(int i=strt; i<lenth; i++)
        {
            if(data2[i]==':'&&flag==0)
            {
                cinfo.alias_name2=data2.substr(0,i);
                strt=i+1;
                flag=1;
                continue;
            }

            if(data2[i]==':'&&flag==1)
            {
                cinfo.client_ip=data2.substr(strt,i-strt);
                strt=i+1;
                flag=2;
                continue;
            }
            if(data2[i]==':'&&flag==2)
            {
                cinfo.client_port=data2.substr(strt,i-strt);
                strt=i+1;
                continue;
            }
            cinfo.downloading_port=data2.substr(strt,len-strt);


        }

        c.push_back(cinfo);
    }

    pthread_t threads[4];

    struct sockaddr_in server_address;
    int sockt;
    socklen_t size;


    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0)
    {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }


    cout << "\n=> Socket server has been created..." << endl;


    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(crs.server_port );


    if ((bind(client, (struct sockaddr*)&server_address,sizeof(server_address))) < 0)
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    size = sizeof(server_address);
    cout << "=> Looking for clients..." << endl;

    int clientCount = -1;
    listen(client,4);
    while(1)
    {
        sockt = accept(client,(struct sockaddr *)&server_address,&size);

        int status;
        status = pthread_create(&threads[clientCount], NULL, thread_create, (void *)&sockt);
        if(status != 0)
            cout <<"Error creating thread\n";
        clientCount++;
    }

    close(client);
    return 0;
}
void *thread_create(void *sock)
{
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char buffer1[bufsize];

    struct sockaddr_in server_address;
    int server= *(int *)sock;
    //if((send(server, buffer, bufsize, 0)) < 0)
    //  cout << "send not sent";

    memset(&buffer, 0, sizeof(buffer));
    cout << "Connected with the client"<< endl;
    while(1)
    {
        fstream afile4;
        string can;
        //recv(server,buffer2,bufsize,0);
        //send(server," ",bufsize,0);
        memset(&buffer1, 0, sizeof(buffer1));
        recv(server, buffer1, bufsize, 0);
        send(server," ",bufsize,0);
        can=buffer1;
        // string clientfile=buffer1;
        // int p=0;
        // while(clientfile[p]!=':'){
        //     p++;
        // }


        // can=clientfile.substr(0,p-1);
        // afile4.open(crs.client_list_file, ios::out | ios::app);
        // afile4<<clientfile;
        recv(server, buffer, bufsize, 0);
        cout<<buffer<<endl;
        string data = buffer;
        int len = data.length();
        //int strt=0;
        // cout<<"Data="<<data<<":length="<<len<<endl;
        fstream  afile2;
        fstream afile3;
        int flag=0;
        string x;
        string y;
        for(int i=0; i<len; i++)
        {

            if(data[i]==' ')
            {
                y=" ";
                x=data.substr(0,i);
                /*===========================EXEC==============================*/
                /*===========================EXEC==============================*/

                if(x.compare("exec")==0)
                {
                    string cmd;
                    string ab;
                    int sp;
                    for(int j=i+2; j<len; j++)
                    {
                        if(data[j]=='"'&& flag==0)
                        {
                            ab=data.substr(i+2,j-i-2);
                            flag=1;
                            sp=j;
                        }

                        if(data[j]=='"'&& flag==1)
                        {
                            cmd=data.substr(sp+3,j-sp-3);
                        }

                    }
                    int pid=fork();
                    if(pid==0)
                    {
                        int pd=0;
                        execl("/bin/bash","bash","-c",cmd.c_str(),(char *)NULL);
                        waitpid(pid,&pd,0);
                    }
                }
                /*===========================DEL==============================*/
                /*===========================DEL==============================*/
                if(x.compare("del")==0)
                {
                    size_t found = data.find_last_of("/\\");
                    string pn=data.substr(i+2,found-i-2);
                    string fn=data.substr(found+1,len-found-2);
                    //cout<<repos.path_name<<endl;
                    //cout<<repos.file_name<<endl;
                    string cn=can;
                    //string to_be_deleted=repos.file_name+":"+repos.path_name+"/"+repos.file_name+":"+repos.alias_name;
                    for(int i=0; i<v.size(); i++)
                    {
                        if(v[i].file_name==fn&&v[i].path_name==pn&&v[i].alias_name==cn)
                        {
                            v.erase(v.begin()+i-1,v.begin()+i);
                        }
                    }
                    for(int i=0; i<v.size(); i++)
                    {
                        cout<<v[i].file_name<<v[i].path_name<<endl;
                    }
                }


                /*===========================SHARE==============================*/
                /*===========================SHARE==============================*/

                if(x.compare("share")==0)
                {


                    //send(server, "buffer", bufsize, 0);
                    size_t found = data.find_last_of("/\\");
                    repos.path_name=data.substr(i+2,found-i-2);
                    //cout<<repos.path_name<<endl;
                    repos.file_name=data.substr(found+1,len-found-2);
                    //cout<<repos.file_name<<endl;
                    repos.alias_name=can;
                    afile2.open(crs.repo_file, ios::out | ios::app);
                    afile2<<repos.file_name<<':'<<repos.path_name<<'/'<<repos.file_name<<':'<<can<<endl;
                    send(server,"SUCCESS:FILE_SHARED",bufsize,0);
                }
                /*===========================SEARCH==============================*/
                /*===========================SEARCH==============================*/
                if(x.compare("search")==0)
                {
                    string s;
                    s=data.substr(i+2,len-i-3);
                    //cout<<"string is:"<<s<<endl;
                    int flag1=0;
                    int count =0;
                    for(int k=0; k<v.size(); k++)
                    {
                        locale loc;
                        if (v[k].file_name.find(s) != string::npos)
                        {
                            //cout<<v[k].file_name<<":"<<v[k].path_name<<":"<<v[k].alias_name<<endl;
                            v1.push_back(v[k]);
                            flag1=1;
                        }

                    }
                    if(flag1==0){
                        //cout<<"dcssdd";
                        send(server,"SEARCH NOT FOUND",bufsize,0);
                        break;
                    }
                    //cout<<y<<"\n-----end----\n"<<endl;
                    for(int k=0; k<v1.size(); k++)
                    {
                        for(int l=0; l<c.size(); l++)
                        {
                            //cout << v1[k].alias_name << " : " << c[l].alias_name2 << "\n";
                            if(v1[k].alias_name == c[l].alias_name2)
                            {
                                count++;
                                c1.push_back(c[l]);

                                y+="["+to_string(count)+"] "+v1[k].file_name+":"+v1[k].path_name+":"+v1[k].alias_name+":"+c[l].client_ip+":"+c[l].client_port+":"+c[l].downloading_port+"\n";

                                //send(server,x.c_str(),bufsize,0);

                                //cout<<v1[k].file_name<<":"<<v1[k].path_name<<":"<<v1[k].alias_name<<":"<<c[l].client_ip<<":"<<c[l].client_port<<":"<<c[l].downloading_port<<endl;
                            }

                        }
                    }
                    //cout<<y<<endl;
                    send(server,y.c_str(),bufsize,0);
                    y=" ";
                    v1.clear();
                }
            }

        }
        // if (*buffer == '&')
        // {
        //     send(server, buffer, bufsize, 0);
        //     isExit = true;
        // }
        // recv(server, buffer, bufsize, 0);
        // if (*buffer == '&')
        // {
        //     isExit = true;
        // }

    }
    //while (!isExit);


    close(server);
    cout << "\nGoodbye..." << endl;
    isExit = false;
    exit(1);
}