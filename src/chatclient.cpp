#include"chatclient.h"
#include<regex>
#include<fstream>

using namespace std;

ChatClient::ChatClient(std::string sname,std::string ip,std::string port)
{
    setName(sname);
    setIP(ip);
    setPort(port);
    udpSock=INVALID_SOCKET;
    tcpSock=INVALID_SOCKET;
}
ChatClient::~ChatClient()
{
    closesocket(udpSock);
}

bool ChatClient::setName(const string &sname)
{
    if(sname.empty())
    {
        return false;
    }
    
    name=sname;
    return true;
}

bool ChatClient::setIP(const string &sip)
{
    regex ip_reg("(\\d{1,3}\\.){3}\\d{1,3}");
    if(regex_match(sip,ip_reg))
    {
        ip=sip;
        return true;
    }
    return false;
}

bool ChatClient::setPort(const string &sport)
{
    regex port_reg(R"(\d{4,5})");
    if(regex_match(sport,port_reg))
    {
        port=sport;
        return true;
    }
    return false;
}

void ChatClient::initSocket()
{
    // init wsa 
    WSADATA data;
    WSAStartup(MAKEWORD(2,2),&data);

    if(udpSock!=INVALID_SOCKET)
    {
        printf("close old socket\n");
        closesocket(udpSock);
    }

    // create socket
    udpSock=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(udpSock==INVALID_SOCKET)
    {
        printf("ERROR::ChatClient: fail to creadte socket");
    }
    // set server addr
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(stoi(port));
    serverAddr.sin_addr.S_un.S_addr=inet_addr(ip.c_str());
    // set recv addr
    sockaddr_in RecvAddr;
    RecvAddr.sin_family = AF_INET;
    RecvAddr.sin_port = htons(4001);
    RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(udpSock, (SOCKADDR *)&RecvAddr, sizeof(RecvAddr));
}

void ChatClient::sendUDP(const string &message)
{
    string sendMsg=name+": "+message;
    sendto(udpSock,sendMsg.c_str(),sendMsg.size()+1,0,(SOCKADDR*)&serverAddr,sizeof(serverAddr));
}

string ChatClient::recvUDP()
{
    char recvBuf[1024];
    int BufLen=1024;
    int len=sizeof(serverAddr);
    recvfrom(udpSock,recvBuf,BufLen,0,(sockaddr*)&serverAddr,&len);
    string msg(recvBuf);
    return msg;
}

void ChatClient::closeSocket()
{
    closesocket(udpSock);
    WSACleanup();
}

bool ChatClient::sendFile(const std::string path)
{
    // open file
    ifstream in(path.c_str(),ios::in | ios::binary);
    if(!in)
    {
        printf("ERROR::sendFile: can't open file! \n");
        return false;
    }

    //establish connect to server
    tcpSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(tcpSock==INVALID_SOCKET)
    {
        printf("ERROR::sendFile: can't init socket!\n");
        return false;
    }
    if(connect(tcpSock,(sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR)
    {
        printf("ERROR::sendFile: can't connect to server");
        closesocket(tcpSock);
        return false;
    }

    // send file
    printf("start to send file--------------------------\n");
    const int bufsize=1024;
    char bufchar[bufsize]={0};
    while(!in.eof())
    {
        in.read(bufchar,bufsize);
        send(tcpSock,bufchar,bufsize,0);
        printf("--------------------------------\n");
    }
    closesocket(tcpSock);
    in.close();
    printf("----------------------------------finish\n");

}

bool ChatClient::recvFile(const std::string path)
{
    // open file
    ofstream out(path.c_str(),ios::out | ios::binary);
    if(!out)
    {
        printf("ERROR::recvFile: can't open file! \n");
        return false;
    }

    //establish connect to server
    tcpSock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(tcpSock==INVALID_SOCKET)
    {
        printf("ERROR::recvFile: can't init socket!\n");
        return false;
    }
    if(connect(tcpSock,(sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR)
    {
        printf("ERROR::recvFile: can't connect to server");
        closesocket(tcpSock);
        return false;
    }

    // recv file
    printf("start to recv file--------------------------\n");
    const int bufsize=1024;
    char bufchar[bufsize]={0};
    int readlen;
    while(true)
    {
        readlen=recv(tcpSock,bufchar,bufsize,0);
        if(readlen==0)  // file recv finished
        {
            break;
        }
        out.write(bufchar,bufsize);
        printf("--------------------------------\n");
    }
    closesocket(tcpSock);
    out.close();
    printf("----------------------------------finish\n");
}