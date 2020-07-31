#ifndef CHATCLIENT_H
#define CHATCLIENT_H


#include <WinSock2.h>
#include<string>
#pragma comment(lib, "WS2_32.lib")

class ChatClient
{
private:
    std::string name; // name of client
    std::string ip; // ip of server
    std::string port;   // port of server
    SOCKET udpSock;
    SOCKET tcpSock;
    sockaddr_in serverAddr;
public:
    ChatClient(std::string sname,std::string ip,std::string port);
    ~ChatClient();

    void initSocket();  
    void closeSocket();
    bool setName(const std::string &sname);
    bool setPort(const std::string &sport);
    bool setIP(const std::string &ip);
    void sendUDP(const std::string &message);
    std::string recvUDP();
    bool sendFile(const std::string path);
    bool recvFile(const std::string path);

};


#endif