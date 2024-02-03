#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 512
//////////////////////////////////////////////////
#define MAX_MESSAGE_LENGTH 2000


/////////////////////////////////////////////////
typedef struct
{
    char* ip; // server's ip
    unsigned short port; // server's port
    int conn_fd; // fd to talk with server
    char buf[BUFFER_SIZE]; // data sent by/to server
    size_t buf_len; // bytes used by buf
} client;

client cli;

static void init_client(char** argv);
////////////////////////////////////////////////////

void sendM(char* message);
char* receiveM();


int main(int argc, char** argv)
{
    char command[5];
    char msg[MAX_MESSAGE_LENGTH];

    char str1[MAX_MESSAGE_LENGTH];
    char str2[MAX_MESSAGE_LENGTH];

    char temp[MAX_MESSAGE_LENGTH];
    char tp;



    // Parse args.
    if(argc!=3)
    {
        ERR_EXIT("usage: [ip] [port]");
    }

    // Handling connection
    init_client(argv);
    strcpy(msg,receiveM());



    //fprintf(stderr, "connect to %s %d\n", cli.ip, cli.port);
    fprintf(stdout,"%s",msg);
    fflush(stdout);



    while(1)
    {
        // TODO: handle user's input
        fprintf(stdout,"%s","Please enter your command (post/pull/exit): ");
        fflush(stdout);
        //printf("Please enter your command (post/pull/exit): ");
        scanf("%s",command);
        //printf("your Name: %s\n",command);

        sendM(command);



        if(strcmp(command,"post")==0)
        {


            //fprintf(stderr, "%s\n", "post");
            strcpy(msg,receiveM());

            if(strcmp(msg,"0")==0)
            {
                fprintf(stdout, "%s\n","[Error] Maximum posting limit exceeded");
                fflush(stdout);
            }else
            {

                //printf("FROM: ");
                fprintf(stdout, "%s","FROM: ");
                fflush(stdout);
                //scanf("%c",&tp);//?
                //scanf("%[^\n]",str1);
                //scanf("%[a-zA-Z0-9_,.]",str1);
                scanf("%s",str1);
                ///////////////////////////////////////

                //printf("CONTENT:\n");
                fprintf(stdout, "%s","CONTENT:\n");
                fflush(stdout);
                //scanf("%c",&tp);
                //scanf("%[^\n]",str2);
                //scanf("%[a-zA-Z0-9_,.]",str2);
                scanf("%s",str2);
                //////////////////////////////////////////////////////


                strcpy(temp,"[SP]");
                strcat(temp,str1);
                strcat(temp,"+");
                strcat(temp,str2);

                //fprintf(stdout,"%s\n",temp);

                sendM(temp);

            }




        }else if(strcmp(command,"pull")==0)
        {
            strcpy(msg,receiveM());

            //fprintf(stderr, "%s\n", "pull");
            fprintf(stdout, "%s", msg);
            fflush(stdout);
            continue;

        }else
        {
            break;
        }

    }

}
////////////////////////////////////////////////////////////////////////

void sendM(char* message)
{

     // set the IP address and port number for the server
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(cli.ip);
    server_address.sin_port = htons(cli.port);

    // send a message to the server
    socklen_t server_address_size = sizeof(server_address);
    if (sendto(cli.conn_fd, message, strlen(message), 0, (struct sockaddr*)&server_address, server_address_size) < 0)
    {
        perror("Error sending message to server");
        exit(1);
    }

}

char* receiveM()
{
    struct sockaddr_in server_address;

    socklen_t server_address_size = sizeof(server_address);

   // receive a response from the server
    static char response[MAX_MESSAGE_LENGTH];
    memset(response, 0, MAX_MESSAGE_LENGTH);
    if (recvfrom(cli.conn_fd, response, MAX_MESSAGE_LENGTH, 0, (struct sockaddr*)&server_address, &server_address_size) < 0)
    {
        perror("Error receiving message from server");
        exit(1);
    }

    //response[MAX_MESSAGE_LENGTH-1] = '\0';

    return response;
}


//////////////////////////////////////////////////////////////////////
static void init_client(char** argv)
{

    cli.ip = argv[1];

    if(atoi(argv[2])==0 || atoi(argv[2])>65536)
    {
        ERR_EXIT("Invalid port");
    }
    cli.port=(unsigned short)atoi(argv[2]);

    struct sockaddr_in servaddr;
    cli.conn_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(cli.conn_fd<0)
    {
        ERR_EXIT("socket");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(cli.port);

    if(inet_pton(AF_INET, cli.ip, &servaddr.sin_addr)<=0)
    {
        ERR_EXIT("Invalid IP");
    }

    if(connect(cli.conn_fd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {
        ERR_EXIT("connect");
    }

    return;
}
