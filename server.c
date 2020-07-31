#include<stdio.h>
#include<winSock2.h>
#pragma comment(lib, "ws2_32.lib")

void sendhtml(SOCKET s, char *filename);
int merror(int redata, long long error, char* showinfo){
    if(redata == error){
        perror(showinfo);
        getchar();
        return -1;
    }
    return 0;
}

int main(){
    printf("Welcome to  my web server!\n");
    WSADATA wsdata;
    int isok = WSAStartup(MAKEWORD(2, 2), &wsdata);//确定socket版本信息
    merror(isok, WSAEINVAL, "Fail to apply for socke");
    
    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//IPV4地址；传输类型，流传输；传输协议 TCP
    merror(server, INVALID_SOCKET, "Fail to creat socket");

    struct sockaddr_in seraddr;
    seraddr.sin_family= AF_INET;
    seraddr.sin_port = htons(80);
    seraddr.sin_addr.s_addr = INADDR_ANY;

    isok = bind(server, (struct sockaddr*) &seraddr, sizeof(seraddr));/*函数中需将 sockaddr_in 强制转换为 sockaddr*/
    merror(isok, SOCKET_ERROR, "Fail to bind");

    isok = listen(server, 5);
    merror(isok, SOCKET_ERROR, "fail to listen");
    
    struct sockaddr claddr;
    int cllen = sizeof(claddr);

    while(1){
        SOCKET client = accept(server, (struct sockaddr*) &claddr, &cllen);/*函数中需将 sockaddr_in 强制转换为 sockaddr*/
        merror(client, INVALID_SOCKET, "Fail to connect");

        char revdata[1024] = "";
        recv(client, revdata, 1024, 0);
        printf("%s %d bytes reveived", revdata, strlen(revdata));
        
        //char sendata[1024] = "<h1 style=\" color:red;\">Hello, this is my web<h1>";
        //send(client, sendata, strlen(sendata), 0);
        char *filename = "./index.html";
        sendhtml(client, filename);
        closesocket(client);
    }
    closesocket(server);
    WSACleanup();
    getchar();
    return 0;
}

void sendhtml(SOCKET s, char *filename){
    FILE *pfile = fopen(filename, "r");
    if(pfile == NULL){
        printf("fail to open file");
        return;
    }
    char temp[1024] = "";
    do{
    fgets(temp, 1024, pfile);
    send(s, temp, strlen(temp), 0);
    } while(!feof(pfile));
}