#include<iostream>
#include<string>
#include<thread>
#include"chatclient.h"

using namespace std;

void listenThread(ChatClient &client)
{
    std::string msg;
    while(1)
    {
        msg=client.recvUDP();
        cout<<"message from server:-------"<<msg<<endl<<endl;
    }
}


int main()
{

    cout<<"输入名字，服务器ip，端口："<<endl;
    string name,ip,port;
    cin>>name;
    ip="192.168.31.168";
    port="4000";
    
    getchar();

    ChatClient client(name,ip,port);
    client.initSocket();
    // start to listen
    thread listen(listenThread,ref(client));
    // send message
    while(1)
    {
        string sendMsg;
        if(getline(cin,sendMsg))
        {
            if(sendMsg=="exit")
            {
                client.closeSocket();
            }
            client.sendUDP(sendMsg);
        }
    }

}