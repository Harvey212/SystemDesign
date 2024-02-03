#include "hw1.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>


#define ERR_EXIT(a) do { perror(a); exit(1); } while(0)
#define BUFFER_SIZE 512
/////////////////////////////////////////
////edited
#define MAX_MESSAGE_LEN 2000

//start from here
////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct
{
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[BUFFER_SIZE];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    int id;
} request;
//////////////////////////////////////////////////////////////////////////////////////////

server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list

// initailize a server, exit for error
static void init_server(unsigned short port);

// initailize a request instance
static void init_request(request* reqP);

// free resources used by a request instance
static void free_request(request* reqP);
////////////////////////////////////////////////////////////////////////


char* receiveM(int socket_fd, struct sockaddr_in client_address);
void sendM(int socket_fd, struct sockaddr_in client_address,char* msg);
void removeSubstr (char *string, char *sub);
///////////////////////////////////////////////////////////////////
//close(requestP[conn_fd].conn_fd);
//free_request(&requestP[conn_fd]);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{


    // Parse args.
	if (argc != 2)
    {
		ERR_EXIT("usage: [port]");
        exit(1);
	}

	struct sockaddr_in cliaddr;  // used by accept()
	int clilen;

	int new_conn_fd;  // fd for a new connection with client
	int file_fd;  // fd for file that we open for reading
	char buf[BUFFER_SIZE];
	int buf_len;

	// Initialize server
	init_server((unsigned short) atoi(argv[1]));


    // Loop for handling connections
    //fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);

    /////////////////////////////////////////////////////////
	// Initialize

	//strcat(,)
	//"BulletinBoard"
	file_fd = open(RECORD_PATH,O_RDWR);
    char command[MAX_MESSAGE_LEN];

	fd_set read_fds,master;
	FD_ZERO(&master);
	FD_SET(svr.listen_fd, &master);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1;
    int sel;




    char str1[MAX_MESSAGE_LEN];
    char str2[MAX_MESSAGE_LEN];

    char temp[MAX_MESSAGE_LEN];

    const char* d = "+";
    char *p;

    char *ret;

    //fprintf(stderr, "%u\n", recordsize);
    ////////////////////////////////////////////////////////////////////////
    //conclusion2:
    //1.even write lock, no matter how, read still>0;
    //2.if you use read() to read a space not hasn't been written, it still returns >0
    //3.don't use GETLK, it would turn to F_UNLCK anyway even you have set WRLOCK
    //4.even you have set the lock, if other server or process want to change to different type lock,they can change to theirs easily
    /////////////////////////////ONLY usable part
    //5.if you use fnctl to lock, if other process also want to use same lock, then return -1, unless your server unlock
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////debug use
    //struct Record re;
    //strcpy(re.From,"TA1");
    //strcpy(re.Content,"TT");
    //struct flock lock1;
    //write(file_fd, &re, recordsize);

    //lock1.l_type = F_WRLCK;
    //lock1.l_start = 0;
    //lock1.l_whence = SEEK_SET;
    //lock1.l_len = recordsize;


    //int see=fcntl(file_fd, F_SETLK, &lock1);
    //fprintf(stderr, "%d\n", see);
    //fcntl(file_fd, F_GETLK, &lock1);

    //if (lock1.l_type == F_WRLCK)
    //{
    //    fprintf(stderr, "%s\n", "F_WRLCK");
    //}else if(lock1.l_type == F_UNLCK)
    //{
    //    fprintf(stderr, "%s\n", "F_UNLCK");
    //}else if(lock1.l_type == F_RDLCK)
    //{
    //    fprintf(stderr, "%s\n", "F_RDLCK");
    //}
    ////////////////////////////////////////////////////////////////////////
    record r0;
    long int recordsize = sizeof(r0);
    //strcpy(r0.From,"HI");
    //strcpy(r0.Content,"koo");

    //fprintf(stderr,"%s",r0.From);
    //fprintf(stderr,"%s",r0.Content);
    //fprintf(stderr,"%ld",recordsize);
    ////////////////////////////////////////////////////
    //struct flock lock[maxfd];
    struct flock tplk;
    int mylock[RECORD_NUM] = {0};  //lock:1 nolock=0


    int c=0;
    int see=0;

    int last=-1;

    int LAST[1024];
    //int LAST[maxfd];

    //memset(LAST,0,maxfd);

    //fprintf(stderr,"%d",maxfd);

    int unlkc=RECORD_NUM;
    int interlk=0;
    int oldlast=last;

    int lockcount=0;
    ///////////////////////////////////////
    //int unlkset[RECORD_NUM]={0};
    //for(int m=0;m<RECORD_NUM;m++)
    //{
    //    fprintf(stderr,"%d",unlkset[m]);
    //}
    //////////////////////////////////////
    ////////////////////////////////
    //sit
    //0: lock
    //1: not lock
    /////////////////////////


	while (1)
    {
		// Add IO multiplexing
		read_fds=master; // readfd=master so far


		// Check new connection
		sel = select(maxfd+1,&read_fds,NULL,NULL,&timeout);  //after this function, readfd would change, only the fd who receive message would remain
        /////////////////////////////////////
        //conclusion:(after select)  => note that we only set a ear(select) at the server side
        //1.when receive new connection, svr.listen_fd will be in read_fds, but client's won't.
        //2.when client send a message, client's fd will be in read_fd, but svr.listen_fd won't
        //3.when client receive message,
        //4.when server send message, nobody in the readfd (probably because it's read fd , and server sending. client no ear)
        //5.when server receive message,
        /////////////////////////////////////

		if(sel>0)
		{

            ///////////////////////////////////////////////////////////
            //only receive new connection would enter this process
            if(FD_ISSET(svr.listen_fd, &read_fds))
            {
                clilen = sizeof(cliaddr);
                //cliaddr before and after accept() would change
                //fprintf(stderr,"%ld\n", strlen(  ((struct sockaddr*)&cliaddr)->sa_data )    );
                new_conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
                //fprintf(stderr,"%ld\n", strlen(  ((struct sockaddr*)&cliaddr)->sa_data )    );
                //fprintf(stderr,"%s\n",((struct sockaddr*)&cliaddr)->sa_data);

                if (new_conn_fd < 0)
                {
                    if (errno == EINTR || errno == EAGAIN) continue;  // try again
                    if (errno == ENFILE)
                    {
                        (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                        continue;
                    }
                    ERR_EXIT("accept");
                }
                requestP[new_conn_fd].conn_fd = new_conn_fd;
                FD_SET(new_conn_fd,&master);
                strcpy(requestP[new_conn_fd].host, inet_ntoa(cliaddr.sin_addr));
                //fprintf(stderr, "getting a new request... fd %d from %s\n", new_conn_fd, requestP[new_conn_fd].host);
                //////////////////////////////////////////////////////////////////////////////////////////

                strcpy(temp,"==============================\n");
                strcat(temp,"Welcome to CSIE Bulletin board\n");
                strcat(temp,"==============================\n");
                //////////////////////////////////////////////////////

                lockcount=0;
                for(int k=0;k<RECORD_NUM;k++)
                {

                    //////////////////////////////////
                    //to check if the place is set by others
                    tplk.l_type = F_WRLCK;
                    tplk.l_start = recordsize*k;
                    tplk.l_whence = SEEK_SET;
                    tplk.l_len = recordsize;
                    see=fcntl(file_fd, F_SETLK, &tplk);

                    ////////////////////////////////////////////////////////

                    if(see<0)
                    {
                        //fprintf(stderr,"%s\n","locked by other server.");
                        lockcount=lockcount+1;
                    }else
                    {
                        if(mylock[k]==0)
                        {
                            tplk.l_type = F_UNLCK;
                            tplk.l_start = recordsize*k;
                            tplk.l_whence = SEEK_SET;
                            tplk.l_len = recordsize;
                            fcntl(file_fd, F_SETLK, &tplk);
                            //////////////////////////////////

                            record re;
                            strcpy(re.From,"");
                            strcpy(re.Content,"");

                            lseek(file_fd, recordsize*k, SEEK_SET);
                            read(file_fd,&re,recordsize);


                            if(strlen(re.Content)!=0)
                            {
                                strcat(temp,"From: ");
                                strcat(temp,re.From);
                                strcat(temp,"\n");
                                strcat(temp,"Content:\n");
                                strcat(temp,re.Content);
                                strcat(temp,"\n");
                            }

                            //lseek(file_fd,0, SEEK_SET);

                        }else
                        {
                            lockcount=lockcount+1;
                            //fprintf(stderr,"%s\n","locked by my server.");
                        }

                    }

                    /////////////////////////////////////////////////////////////////////
                }
                strcat(temp,"==============================\n");
                sendM(requestP[new_conn_fd].conn_fd, cliaddr,temp);


                if(lockcount>0)
                {
                    fprintf(stdout,"[Warning] Try to access locked post - %d\n",lockcount);
                    fflush(stdout);
                }



                ///////////////////////////////////////////////////////////////////////////////////
                continue;
            }
            ////////////////////////////////////////////////////////////
            //if receive message from client
            for(int i=0;i<maxfd;i++)
            {

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    if((requestP[i].conn_fd!=svr.listen_fd) && (FD_ISSET(requestP[i].conn_fd, &read_fds)) )
                    {


                        strcpy(command,receiveM(requestP[i].conn_fd,cliaddr));

                        ret = strstr(command, "[SP]");
                        if (ret)
                        {

                                //fprintf(stderr, "%s\n", "new post reply!");
                                c=0;
                                removeSubstr(command, "[SP]");
                                p = strtok(command, d);

                                while (p != NULL)
                                {

                                    if(c==0)
                                    {
                                        strcpy(str1,p);
                                        c=c+1;
                                    }else
                                    {
                                        strcpy(str2,p);
                                    }

                                    p = strtok(NULL, d);
                                }

                                record re;

                                strcpy(re.From,str1);
                                strcpy(re.Content,str2);

                                //printf("From: %s\n", re.From);
                                //printf("Content: %s\n",re.Content);

                                lseek(file_fd, recordsize*LAST[requestP[i].conn_fd], SEEK_SET);
                                write(file_fd, &re, recordsize);
                                ////////////////////////////////////////////////
                                tplk.l_type = F_UNLCK; //lock[requestP[i].conn_fd]
                                tplk.l_start = recordsize*LAST[requestP[i].conn_fd];
                                tplk.l_whence = SEEK_SET;
                                tplk.l_len = recordsize;

                                lseek(file_fd,0,SEEK_SET);
                                fcntl(file_fd, F_SETLK, &tplk);

                                mylock[last]=0;
                                //////////////////////////////////////////////////////////////
                                fprintf(stdout, "[Log] Receive post from %s\n",re.From);
                                fflush(stdout);

                        }else
                        {

                            if(strcmp(command,"post")==0)
                            {
                                //fprintf(stderr, "%s\n", "new post request!");
                                unlkc=RECORD_NUM;
                                interlk=0;

                                if((last+1)==RECORD_NUM)
                                {
                                    last=-1;

                                }




                                oldlast=last;


                                ///////////////////////////////////
                                for(int m=oldlast+1;m<RECORD_NUM;m++)
                                {

                                    tplk.l_type = F_WRLCK;
                                    tplk.l_start = recordsize*m;
                                    tplk.l_whence = SEEK_SET;
                                    tplk.l_len = recordsize;
                                    see=fcntl(file_fd, F_SETLK, &tplk);

                                    if(see<0)
                                    {
                                            unlkc=unlkc-1;
                                    }else
                                    {

                                        if(mylock[m]==0)
                                        {
                                            if(interlk==0)
                                            {
                                                interlk=interlk+1;
                                                last=m;
                                                LAST[requestP[i].conn_fd]=last;
                                                mylock[last]=1;
                                            }
                                        }else
                                        {

                                            unlkc=unlkc-1;
                                        }

                                    }
                                }


                                if(oldlast+1>0)
                                {
                                    for(int m=0;m<oldlast+1;m++)
                                    {


                                        tplk.l_type = F_WRLCK;
                                        tplk.l_start = recordsize*m;
                                        tplk.l_whence = SEEK_SET;
                                        tplk.l_len = recordsize;
                                        see=fcntl(file_fd, F_SETLK, &tplk);

                                        if(see<0)
                                        {
                                            unlkc=unlkc-1;
                                        }else
                                        {

                                            if(mylock[m]==0)
                                            {
                                                if(interlk==0)
                                                {
                                                    interlk=interlk+1;
                                                    last=m;
                                                    LAST[requestP[i].conn_fd]=last;
                                                    mylock[last]=1;
                                                }
                                            }else
                                            {
                                                unlkc=unlkc-1;
                                            }

                                        }

                                    }

                                }


                                //////////////////////////////////////////

                                if(unlkc==0)
                                {
                                    //If all ten posts in BulletinBoard are locked
                                    sendM(requestP[i].conn_fd, cliaddr,"0");

                                }else
                                {
                                    //If there is an writable post
                                    sendM(requestP[i].conn_fd, cliaddr,"1");
                                }


                            }else if(strcmp(command,"pull")==0)
                            {
                                lockcount=0;

                                //fprintf(stderr, "%s\n", "new pull request!");

                                strcpy(temp,"==============================\n");

                                //fprintf(stderr, "%s\n", "===================================");

                                //fprintf(stderr, "last: %d \n", last);
                                //Collect all unlocked posts from server and print in following format.
                                ////////////////////////////////////////////////////////////////////////////////
                                //lseek would not change the original file fd
                                //If data is not available, and the socket is in nonblocking mode, read() returns a -1

                                //////////////////////////////////////////////////////

                                for(int k=0;k<RECORD_NUM;k++)
                                {

                                    //////////////////////////////////
                                    //to check if the place is set by others
                                    tplk.l_type = F_WRLCK;
                                    tplk.l_start = recordsize*k;
                                    tplk.l_whence = SEEK_SET;
                                    tplk.l_len = recordsize;
                                    see=fcntl(file_fd, F_SETLK, &tplk);

                                    ////////////////////////////////////////////////////////

                                    if(see<0)
                                    {
                                        //fprintf(stderr,"%s\n","locked by other server.");
                                        lockcount=lockcount+1;

                                    }else
                                    {
                                        if(mylock[k]==0)
                                        {
                                            tplk.l_type = F_UNLCK;
                                            tplk.l_start = recordsize*k;
                                            tplk.l_whence = SEEK_SET;
                                            tplk.l_len = recordsize;
                                            fcntl(file_fd, F_SETLK, &tplk);
                                            //////////////////////////////////

                                            //fprintf(stderr,"%d\n",file_fd);
                                            record re;
                                            strcpy(re.From,"");
                                            strcpy(re.Content,"");


                                            lseek(file_fd, recordsize*k, SEEK_SET);
                                            read(file_fd,&re,recordsize);

                                            //fprintf(stderr,"%d\n",k);
                                            if(strlen(re.Content)!=0)
                                            {

                                                strcat(temp,"From: ");
                                                strcat(temp,re.From);
                                                strcat(temp,"\n");
                                                strcat(temp,"Content:\n");
                                                strcat(temp,re.Content);
                                                strcat(temp,"\n");
                                            }

                                            //lseek(file_fd,0, SEEK_SET);

                                        }else
                                        {
                                            lockcount=lockcount+1;
                                            //fprintf(stderr,"%s\n","locked by my server.");
                                        }

                                    }

                                    /////////////////////////////////////////////////////////////////////
                                }
                                strcat(temp,"==============================\n");
                                sendM(requestP[i].conn_fd, cliaddr,temp);
                                ///////////////////////////////////////////
                                if(lockcount>0)
                                {
                                    fprintf(stdout,"[Warning] Try to access locked post - %d\n",lockcount);
                                    fflush(stdout);
                                }



                            }else if(strcmp(command,"exit")==0)
                            {
                                //fprintf(stderr, "%s\n", "new exit request!");
                                FD_CLR(requestP[i].conn_fd, &master);
                                close(requestP[i].conn_fd);
                                free_request(&requestP[i]);
                            }else
                            {
                                //fprintf(stderr, "%s\n", "wrong");
                            }

                        }

                    }
////////////////////////////////////////////////////////////////////////////////////////////////

            }

		}
		////////////////////////////////////////////////////


	}

	close(file_fd);
	free(requestP);
	return 0;
}






///////////////////////////////////////////////////////////////////////////////////////////////////////
void removeSubstr (char *string, char *sub)
{
    char *match = string;
    int len = strlen(sub);
    while ((match = strstr(match, sub)))
    {
        *match = '\0';
        strcat(string, match+len);
        match++;
    }
}








//////////////////////////////////////////////////////////////////////////////
// receive a message from the client
char* receiveM(int socket_fd, struct sockaddr_in client_address)
{
    static char message[MAX_MESSAGE_LEN];
    memset(message, 0, MAX_MESSAGE_LEN);
    socklen_t client_address_size = sizeof(client_address);

    if (recvfrom(socket_fd, message, MAX_MESSAGE_LEN, 0, (struct sockaddr*)&client_address, &client_address_size) < 0)
    {
        perror("Error receiving message from client");
        exit(1);
    }
    //message[MAX_MESSAGE_LEN-1] = '\0';


    return message;


    //fprintf(stderr,"%s\n",message);
    //printf("my message is :%s \n", message);

}


void sendM(int socket_fd, struct sockaddr_in client_address,char* msg)
{

    socklen_t client_address_size = sizeof(client_address);

    char response[MAX_MESSAGE_LEN];
    memset(response, 0, MAX_MESSAGE_LEN);
    strncpy(response, msg, MAX_MESSAGE_LEN-1);
    if (sendto(socket_fd, response, strlen(response), 0, (struct sockaddr*)&client_address, client_address_size) < 0)
    {
        perror("Error sending message to client");
        exit(1);
    }

}

// ======================================================================================================
// You don't need to know how the following codes are working

static void init_request(request* reqP)
{
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->id = 0;
}

static void free_request(request* reqP)
{
    init_request(reqP);
}

static void init_server(unsigned short port)
{
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;

    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0)
    {
        ERR_EXIT("setsockopt");
    }

    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        ERR_EXIT("bind");
    }

    if (listen(svr.listen_fd, 1024) < 0)
    {
        ERR_EXIT("listen");
    }

    // Get file descripter table size and initialize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);

    if (requestP == NULL)
    {
        ERR_EXIT("out of memory allocating all requests");
    }

    for (int i = 0; i < maxfd; i++)
    {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    return;
}
