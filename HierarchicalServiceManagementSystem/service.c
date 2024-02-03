#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "util.h"

#define ERR_EXIT(s) perror(s), exit(errno);
/////////////////////////////////////////////////////////////////////
static unsigned long secret;
static char service_name[MAX_SERVICE_NAME_LEN];

/////////////////////////////////////////////////////////////////////
int childrenNum=0;
Node* head=NULL; //to store your children

////////////////////////////////////////////////////////////////////
//Spawn:
//Method1:after spawn, parent first read and wait children to reply and store in global TEMP, then after child is spawned, he will write to parent
//=>somehow will fail, parent would read return "" 0 byte even though child hasn't send message
//=>still need to figure out know why
//char TEMP[128] = "a";
//////////////////////////////////////////////
//Method2:parent first return to the main function, by then child would have sent message to the pipe to inform it has been formed, then parent find child to read
//=>success
//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////

////////////////////////////////////////////////
static inline bool is_manager()
{
    return strcmp(service_name, "Manager") == 0;
}



void print_not_exist(char *service_name)
{
    printf("%s doesn't exist\n", service_name);
}

void print_receive_command(char *service_name, char *cmd)
{
    printf("%s has received %s\n", service_name, cmd);
}

void print_spawn(char *parent_name, char *child_name)
{
    printf("%s has spawned a new service %s\n", parent_name, child_name);
}

void print_kill(char *target_name, int decendents_num)
{
    printf("%s and %d child services are killed\n", target_name, decendents_num);
}

void print_acquire_secret(char *service_a, char *service_b, unsigned long secret)
{
    printf("%s has acquired a new secret from %s, value: %lu\n", service_a, service_b, secret);
}

void print_exchange(char *service_a, char *service_b)
{
    printf("%s and %s have exchanged their secrets\n", service_a, service_b);
}
///////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// Function to add a child to the parent's linked list
void addChild(Service* newchild)
{
    Node* newNode = (Node*) malloc(sizeof(Node));

    newNode->service = newchild;
    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
    }else
    {
        Node* current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}
//////////////////////////////////////
////////////////////////////////////////////////////////////
void writeToParent(char* msg)
{
    ssize_t bytes_write;

    for(int i=0;i<strlen(msg)+1;i++)
    {
        bytes_write= write(4, &msg[i], 1);
    }





    ////////////////////////////////////////////////////
    //ssize_t bytes_write= write(4, msg, strlen(msg)+1);
    ///////////////////////////////////////////////////
    //char buffer[128];
    //strcpy(buffer, msg);
    //printf("%s\n",buffer);
    //strcat(buffer, "\n");
    ////////////////////
    //int x;
    //x  = (int) strlen(buffer);
    //buffer[x]='\0';
    /////////////////////////
    //printf("start writing to Parent. : %s ## %zu \n",buffer,strlen(buffer));
    //write(4, &buffer, strlen(buffer)+1);//4 //
    //printf("www %lu \n",bytes_write);
    //printf("complete writing to Parent.\n");
}
///////////////////////////////////////////////////
const char* readFromParent()
{


    ///////////////////////////////////////////
    int readfd =3;
    char *response = malloc (sizeof (char) * 128);

    char *temp = malloc (sizeof (char) * 128);

    int p=0;

    while(read(readfd, temp,1))
    {
        if(temp[0]!='\0')
        {
            response[p] = temp[0];
            p+=1;
        }else
        {
            break;
        }
    }


    response[p]='\0';

    //////////////////////////////////
    //ssize_t bytes_read = read(readfd, response, sizeof(response)+1);//3 sizeof(response)+1
    //int sel;
    //int maxfd = getdtablesize();
    //struct timeval timeout;
    //timeout.tv_sec = 0;
    //timeout.tv_usec = 1;
    //fd_set temp_fds,master;
    //FD_ZERO(&master);
    //3
    //FD_SET(readfd, &master);
    ///////////////////////////////////////////////////////////////////////////
    //char *temp = malloc (sizeof (char) * 9);
    //bool gog=true;
    //bool first=true;
    //bool second=true;
    //int count=0;
    //printf("bbbbbbb\n");
    //while(gog)
    //{
    //    temp_fds=master;
    //    sel = select(maxfd+1,&temp_fds,NULL,NULL,&timeout);
    //    strcpy(temp,"");
    //    if((sel>0) && (first))
    //    {
    //        ssize_t bytes_read = read(readfd, temp, sizeof(temp)+1);//3 sizeof(response)+1
    //        first =false;
    //        strcpy(response,temp);
    //        count+=1;
    //    }else
    //    {
    //        if((sel>0) && (second))//
    //        {
    //            ssize_t bytes_read = read(readfd, temp, sizeof(temp)+1);
    //            strcat(response,temp);
    //            second=false;
    //            count+=1;
    //        }else
    //        {
    //            if((!first))
    //            {
    //                gog=false;
    //            }
    //        }
    //    }
    //}
    //printf("zzz  %d\n",count);
    //response[strlen(response)]='\0';
    ///////////////////////////////////////////////
    //char *response = malloc (sizeof (char) * 128);
    //char *temp = malloc (sizeof (char) * 9);
    //bool first=true;
    //while(read(readfd, temp, sizeof(temp)+1)>0)
    //{
    //    if(first)
    //    {
    //        strcpy(response,temp);
            //response[strlen(response)]='\0';
    //        first=false;
    //    }else
    //    {
    //        strcat(response,temp);
            //response[strlen(response)]='\0';
    //    }
    //    p+=1;
    //}
    //response[strlen(response)]='\0';
    //response[p] = '\0';
    //bool gog=true;
    //while(gog)
    //{
    //    temp_fds=master;
    //    sel = select(maxfd+1,&temp_fds,NULL,NULL,&timeout);
    //    if(sel>0)
    //    {
            //printf("hhhhhhhhh\n");
    //        ssize_t bytes_read = read(readfd, response, sizeof(response)+1);//3 sizeof(response)+1
    //        printf("ttttt %lu \n",bytes_read);
    //        if (bytes_read > 0)
    //        {
    //            response[bytes_read] = '\0';
    //            gog=false;
    //        }
    //    }
    //}
    ///////////////////////////////////////////////////////////////////////
    //printf("start reading from Parent.\n");
    //////////////////////////////////////////////
    //ssize_t bytes_read=0;
    //while(bytes_read==0)
    //{
    //    bytes_read = read(3, response, sizeof(response)+1);
    //}
    /////////////////////////////////////
    //ssize_t bytes_read = read(3, response, sizeof(response)+1);//3
    //printf("complete reading from Parent. : %s ## %zu \n",response, bytes_read);
    //if (bytes_read > 0)
    //{
    //    response[bytes_read] = '\0';
        //printf("Parent received: %s\n", response);
    //}
    return response;
}

////////////////////////////////////////////
const char* readFomChild(int readfd)
{
    char *response = malloc (sizeof (char) * 128);
    char *temp = malloc (sizeof (char) * 128);
    int p=0;
    while(read(readfd, temp,1))
    {
        if(temp[0]!='\0')
        {
            response[p] = temp[0];
            p+=1;
        }else
        {
            break;
        }
    }
    response[p]='\0';

    /////////////////////////////////////////////////
    //char *response = malloc (sizeof (char) * 128);
    //////////////////////////////////////////////
    //char *temp = malloc (sizeof (char) * 128);
    //bool first=true;
    //while(read(readfd, temp, sizeof(temp)+1)>0)
    //{
    //    if(first)
    //    {
    //        strcpy(response,temp);
            //response[strlen(response)]='\0';
    //        first=false;
    //    }else
    //    {
    //        strcat(response,temp);
            //response[strlen(response)]='\0';
    //    }
    //    p+=1;
    //}
    //response[strlen(response)]='\0';
    ////////////////////////////////////////////
    //int sel;
    //int maxfd = getdtablesize();
    //struct timeval timeout;
    //timeout.tv_sec = 0;
    //timeout.tv_usec = 1;
    //fd_set temp_fds,master;
    //FD_ZERO(&master);
    //FD_SET(readfd, &master);
    //char *response = malloc (sizeof (char) * 128);
    //ssize_t bytes_read = read(readfd, response, sizeof(response)+1);//3 sizeof(response)+1
    //char *temp = malloc (sizeof (char) * 9);
    //bool gog=true;
    //bool first=true;
    //bool second=true;
    //int count=0;
    //printf("bbbbbbb\n");
    //while(gog)
    //{
    //    temp_fds=master;
    //    sel = select(maxfd+1,&temp_fds,NULL,NULL,&timeout);
    //    strcpy(temp,"");
    //    if((sel>0) && (first))
    //    {
    //        ssize_t bytes_read = read(readfd, temp, sizeof(temp)+1);//3 sizeof(response)+1
    //        first =false;
    //        strcpy(response,temp);
    //        count+=1;
    //    }else
    //    {
    //        if((sel>0) && (second))//
    //        {
    //            ssize_t bytes_read = read(readfd, temp, sizeof(temp)+1);
    //            strcat(response,temp);
    //            second=false;
    //            count+=1;
    //        }else
    //        {
    //            if((!first))
    //            {
    //                gog=false;
    //            }
    //        }
    //    }
    //}
    //printf("zzz  %d\n",count);
    //response[strlen(response)]='\0';
    //response[bytes_read] = '\0';
    ///////////////////////////////////////////////////
    return response;
}
///////////////////////////////////////////////
void writeToChild(int writefd,char* msg)
{


    ssize_t bytes_write;


    for(int i=0;i<strlen(msg)+1;i++)
    {
        bytes_write= write(writefd, &msg[i], 1);
    }





    ///////////////////////////////////////////////
    //ssize_t bytes_write=write(writefd, msg, strlen(msg)+1);

    /////////////////////////////////////////////////////////////
    //char buffer[128];
    //strcpy(buffer, msg);
    //////////////////////////////////
    //strcat(buffer, "\n");
    //int x;
    //x  = (int) strlen(buffer);
    //buffer[x]='\0';
    //printf("%s\n",buffer);
    //strcpy(buffer, concat(&msg, "\n"));
    ///////////////////////////////////
    //printf("start writing to child. : %s ## %zu \n",buffer,strlen(buffer));
    //write(writefd, &buffer, strlen(buffer)+1);
    //printf("complete writing to child.\n");

}
////////////////////////////////////////////////



void MySpawn(char* new_child_name)
{

    /////////////////////////////////
    //to solve dup2(3,3) and dup2(4,4)
    int fakepipe[2];
    pipe2(fakepipe,O_CLOEXEC);
    //////////////////////////////////////
    // Create pipes
    int pipe_from_child[2];
    int pipe_to_child[2];

    pipe2(pipe_from_child,O_CLOEXEC);
    pipe2(pipe_to_child,O_CLOEXEC);


    //////////////////////////////

    close(fakepipe[0]);
    close(fakepipe[1]);
    //////////////////////////////////////////
    //printf("(%d,%d,%d,%d)\n",pipe_to_child[0],pipe_to_child[1],pipe_from_child[0],pipe_from_child[1]);
    //////////////////////////////////////


    // Fork a child process
    pid_t child_pid = fork();

    if (child_pid == 0)
    {
        // Child process
        // Close unused ends of the pipes
        close(pipe_to_child[1]);
        close(pipe_from_child[0]);

        // Duplicate pipe ends to the appropriate file descriptors
        dup2(pipe_to_child[0], 3);//3
        dup2(pipe_from_child[1], 4);//4

        //////////////////////////////////////////
        //NO CLOSE
        // Close original pipe ends
        //close(pipe_to_child[0]);
        //close(pipe_from_child[1]);
        //////////////////////////////////////////

        // Execute the new child service
        execlp("./service", "./service", new_child_name, NULL);

        //return child_pid;
        // execlp() will only return if there is an error
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (child_pid > 0)
    {
        // Parent process
        // Close unused ends of the pipes
        close(pipe_to_child[0]);
        close(pipe_from_child[1]);


         ///////////////////////////////////////////////
        //SPAWM METHOD1: FAIL => would return "" somehow
        //printf("cccccccccc\n");
        //strcpy(TEMP,readFomChild(pipe_from_child[0]));
        //////////////////////////////////////////////////


        // Set up the new child service
        Service* new_child = malloc(sizeof(Service));

        strcpy(new_child->name, new_child_name);
        new_child->pid = child_pid;
        new_child->read_fd = pipe_from_child[0];//pipe_from_child[0];
        new_child->write_fd = pipe_to_child[1];//pipe_to_child[1];

        // Add the new child to the parent's linked list
        addChild(new_child);




        ///////////////////////////////////////////
        //NO CLOSE
        // Close original pipe ends
        //close(pipe_to_child[1]);
        //close(pipe_from_child[0]);
        //return child_pid;
        //////////////////////////////////
    }else
    {
    //    return -1;
        perror("fork");
        exit(EXIT_FAILURE);
    }

}



//////////////////////////////////////////

void deleteNode(char* searchname)
{

    int found=0;
    //temp is used to freeing the memory
    Node* temp3;

    //key found on the head node.
    //move to head node to the next and free the head.

    if(strcmp((head->service)->name, searchname) == 0)
    {
        close((head->service)->read_fd);
        close((head->service)->write_fd);



        temp3 = head;    //backup to free the memory
        head = (head)->next;
        free(temp3);

          found=1;
    }else
    {
          Node* current  = head;

          while(current->next!= NULL)
          {
            //if yes, we need to delete the current->next node

            if(strcmp(((current->next)->service)->name, searchname) == 0)
            {



                close(((current->next)->service)->read_fd);
                close(((current->next)->service)->write_fd);


                temp3 = current->next;
                //node will be disconnected from the linked list.
                current->next = current->next->next;
                free(temp3);
                found=1;
                break;
            }else
            {
                current = current->next;
            }

          }
    }

    if(found==1)
    {
        childrenNum--;
    }


//    return found;

}
//////////////////////////////////////////////


char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);

    result[strlen(result)]='\0';
    return result;
}



char* intTostr(int num)
{
    int length = snprintf( NULL, 0, "%d",num);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%d",num);
    str[strlen(str)]='\0';
    return str;
}


void die()
{
    /////////////////////////
    Node* curr = head;
    Node* nexx;

    while(curr!= NULL)
    {
        nexx = curr->next;

        ////////////////////////
        Service* ser3 = curr->service;
        deleteNode(ser3->name);

        /////////////////////////////////
        //free(curr);
        curr = nexx;
    }

    head = NULL;
    //////////////////////////////////////
    //////////////////////////

    exit(0);
}





//////////////////////////////////////////////////
int main(int argc, char *argv[])
{


    ///////////////////////////////////////////////////////////////
    pid_t pid = getpid();

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./service [service_name]\n");
        return 0;
    }

    srand(pid);
    secret = rand();
    /*
     * prevent buffered I/O
     * equivalent to fflush() after each stdout
     */
    setvbuf(stdout, NULL, _IONBF, 0);

    strncpy(service_name, argv[1], MAX_SERVICE_NAME_LEN);

    printf("%s has been spawned, pid: %d, secret: %lu\n", service_name, pid, secret);
    ////////////////////////////////////////////////////////////////////////////////////

    //sleep(3);
    //printf("ffffffffff\n");
    if(!is_manager())
    {

        char bu[128];
        strcpy(bu,"Y");
        bu[strlen(bu)]='\0';
        ////////////////////////////

        writeToParent(bu);
    }
    //printf("hhhhhhhhhhh\n");


    //////////////////////////////
    char ansY[128];
    strcpy(ansY,"Y");
    ansY[strlen(ansY)]='\0';

    ///////////////////
    char ansN[128];
    strcpy(ansN,"N");
    ansN[strlen(ansN)]='\0';
    /////////////////////////

    ///////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
    if(is_manager())
    {
    //////////////////////////////////////////
        //for reading input
        int m;
        ///////////
        int ind = 0;
        ///////////
        int num=0;
        ////////////////////
        //SPAWN METHOD2
        int rd=-1;
        ///////////////////////

        //////////////////////////////

        while(1)
        {

            num=0;
            m=0;


            char command[128];
            char* token;
            char* rest;

            char t1[128];
            char t2[128];
            char t3[128];
            char ch;
            char message[128];
            char reply[128];

        ////////////////////////////////////////////////
            char * myfifo; //= "AToB";
            char * myfifo2; //= "BToA";




            ////////////////////////////////////////////
            //for reading input
            //strcpy(command,"");

            //fgets(command, sizeof(command), stdin);
            //////////////////////////////////////
            //scanf("%s", command);
            //m = strlen(command);      // length of user input till first space
            //do
            //{
            //    scanf("%c", &c);
            //    command[m++] = c;       // reading characters after first space (including it)
            //} while (c != '\n');     // until user hits Enter
            //command[m - 1] = 0;       // string terminating

            //printf("%s\n",command);
            /////////////////////////////////////////////////

            ch=fgetc(stdin);

            while((m<128-1) && (ch!=EOF) && (ch!='\n'))
            {
                command[m++]=ch;
                ch=fgetc(stdin);

            }
            //printf("ee %d\n",feof(stdin));

            command[m]='\0';

            //printf("%s\n",command);

            /////////////////////
            strcpy(message, command);
            printf("%s has received %s\n",service_name,message);







            ////////////////////////////////////////////////////////////////////////
            rest = command;
            ind=0;

            char *array[4];

            while ((token = strtok_r(rest, " ", &rest)))
            {
                array[ind] = token;
                ind++;
            }

            rest=array[(ind-1)];
            array[(ind-1)] = strtok_r(rest, "\n", &rest);
            ///////////////////////////////////////////////////

            /////////////////////////////////////////////////////////
            //modify kill command, use s1 to pass command afterwards
            if(strcmp(array[0], "kill") == 0)
            {
                strcpy(message, concat(message, " N"));
                message[strlen(message)]='\0';
            }
            /////////////////////////////////////////
            /////////////////////////////////
        //////////////////////////////////////////////////////////////////////
            if(strcmp(array[0], "spawn") == 0)
            {
                //printf("%s\n","spawn service");
                //strcpy(TEMP,"a");
                strcpy(reply, ansN);
                ////////////////////////
                //SPAWN METHOD2
                rd=-1;
                /////////////////////

                if(strcmp(array[1], service_name) == 0)
                {

                    MySpawn(array[2]);
                    //////////////////////////////////////////
                    //SPAWN METHOD2
                    Node* current = head;
                    while (current!= NULL)
                    {

                        Service* ser = current->service;

                        if(strcmp(ser->name,array[2]) == 0)
                        {
                            rd=ser->read_fd;
                            break;
                        }
                        current = current->next;
                    }




                    strcpy(reply,readFomChild(rd));
                    /////////////////////////////////////////
                    //printf("reply: %s\n",reply);

                    if(strcmp(reply,ansY) == 0)
                    {
                        printf("%s has spawned a new service %s\n",service_name,array[2]);
                    }else
                    {
                        //SHOULD NEVER PRINT THIS
                        printf("kk%skk\n",reply);
                        printf("NO RECIEVE: SPAWN %s %s\n",array[1],array[2]);
                    }


                    childrenNum++;
                }else
                {
                    ////////////////////////////////////////
                    //printf("haaaaaa\n");

                    Node* current = head;
                    while (current!= NULL)
                    {

                        Service* ser = current->service;

                        message[strlen(message)]='\0';
                        writeToChild(ser->write_fd, message);


                        strcpy(reply, readFomChild(ser->read_fd));
                        //printf("%s\n",reply);


                        if(strcmp(reply,ansY) == 0)
                        {
                            break;
                        }
                        current = current->next;
                    }

                    //////////////////////////
                    ///////////////////////////////////////

                    if(strcmp(reply,ansN) == 0)
                    {
                        printf("%s doesn't exist\n",array[1]);
                    }else if(strcmp(reply,ansY) == 0)
                    {
                        printf("%s has spawned a new service %s\n",array[1],array[2]);
                    }else
                    {
                        //SHOULD NEVER PRINT THIS
                        printf("NOOO: kk%skk \n",reply);
                    }
                    /////////////////////////////////

                }

            }else if(strcmp(array[0], "kill") == 0)
            {
                //printf("%s\n","kill service");
        ///////////////////////////////////////////////////////////
                //reply="0";
                strcpy(reply, "0");
                num=0;

                if(strcmp(array[1], service_name) == 0)
                {


                    strcpy(t1, concat(array[0], " "));
                    strcpy(t2, concat(t1,array[1]));
                    strcpy(message, concat(t2,"  Y"));

                    message[strlen(message)]='\0';


                    ////////////////////////////////


                    Node* current = head;
                    while (current!= NULL)
                    {

                        Service* ser = current->service;


                        writeToChild(ser->write_fd, message);
                        //reply=readFomChild(ser->read_fd);

                        strcpy(reply, readFomChild(ser->read_fd));
                        num+=atoi(reply);

                        int status;
                        waitpid(ser->pid, &status, 0);

                        current = current->next;
                    }
                    /////////////////////////////////////////////////////




                    /////////////////////////////////////////////////////////

                    printf("%s and %d child services are killed\n",service_name,num);

                    die();
                    break;

                }else
                {

                    ///////////////////////////////
                    message[strlen(message)]='\0';
                    Node* current = head;
                    while (current!= NULL)
                    {

                        Service* ser = current->service;

                        writeToChild(ser->write_fd, message);

                        //reply=readFomChild(ser->read_fd);
                        strcpy(reply, readFomChild(ser->read_fd));

                        num+=atoi(reply);

                        if(num>0)
                        {

                            if(strcmp(ser->name,array[1]) == 0)
                            {
                                int status;
                                waitpid(ser->pid, &status, 0);

                                deleteNode(ser->name);
                            }
                            break;
                        }

                        current = current->next;
                    }
                    //////////////////////////////
                    //////////////////////////////////////////////////////

                    if(num == 0)
                    {
                        printf("%s doesn't exist\n",array[1]);
                    }else
                    {
                        printf("%s and %d child services are killed\n",array[1],atoi(reply)-1);
                    }
                    //////////////////////////////////////////////////////////

                }


            }else if(strcmp(array[0], "exchange") == 0)
            {


                char c1[128];
                char c2[128];

                char m1[128];
                char m2[128];


                int gnum =0;

                strcpy(c1, concat(array[1],"_to_"));
                strcpy(c2, concat(c1,array[2]));

                strcpy(m1, concat(array[2],"_to_"));
                strcpy(m2, concat(m1,array[1]));

                char * myfifo=c2; //= "AToB";
                char * myfifo2=m2; //= "BToA";
                mkfifo(myfifo, 0666);
                mkfifo(myfifo2, 0666);
                //////////////////////////////////
                ///////////////////////////////////////
                char q1[128];
                char q2[128];
                char q3[128];
                char q4[128];
                char q5[128];

                strcpy(q1, concat(array[0], " "));
                strcpy(q2, concat(q1,array[1]));
                strcpy(q3, concat(q2," "));
                strcpy(q4, concat(q3,array[2]));
                strcpy(q5, concat(q4," "));

                /////////////////////////////
                if((strcmp(array[1],service_name) == 0) || (strcmp(array[2],service_name) == 0))
                {
                    gnum+=1;
                }

                    //////////////////////////////////////////////

                Node* current = head;
                while (current!= NULL)
                {
                    strcpy(message, concat(q5,intTostr(gnum)));
                    message[strlen(message)]='\0';
                    //printf("qqqqqqq %s\n",message);

                    Service* ser = current->service;
                    writeToChild(ser->write_fd, message);
                    strcpy(reply, readFomChild(ser->read_fd));

                    gnum=atoi(reply);

                    if(gnum==2)
                    {
                        break;
                    }

                    current = current->next;
                }
                //////////////////////////////////////////


                if((strcmp(array[1],service_name) == 0) || (strcmp(array[2],service_name) == 0))
                {
                    char tm[128];
                    char tp[128];

                    if(strcmp(array[1],service_name) == 0)
                    {
                        int fd2 = open(myfifo2, O_RDONLY);
                        ////////////////////////////////////////
                        read(fd2, tm, sizeof(tm));
                        close(fd2);
                        /////////////////////////////////////////

                        strcpy(tp,intTostr(secret));
                        tp[strlen(tp)]='\0';

                        int fd = open(myfifo, O_WRONLY);
                        write(fd, tp, strlen(tp)+1);
                        close(fd);

                        secret =atoi(tm);
                        printf("%s has acquired a new secret from %s, value: %lu\n",array[1],array[2],secret);


                    }else
                    {

                        int fd2 = open(myfifo, O_RDONLY);
                        ////////////////////////////////////////
                        read(fd2, tm, sizeof(tm));
                        close(fd2);
                        /////////////////////////////////////////

                        strcpy(tp,intTostr(secret));
                        tp[strlen(tp)]='\0';

                        int fd = open(myfifo2, O_WRONLY);
                        write(fd, tp, strlen(tp)+1);
                        close(fd);

                        secret =atoi(tm);
                        printf("%s has acquired a new secret from %s, value: %lu\n",array[2],array[1],secret);
                    }
                    /////////////////////////////////////////////



                }else
                {
                    char nam1[128];
                    char nam2[128];

                    strcpy(nam1, concat(array[1],".txt"));
                    nam1[strlen(nam1)]='\0';

                    strcpy(nam2, concat(array[2],".txt"));
                    nam2[strlen(nam2)]='\0';

                    bool sp2=true;

                    while(sp2)
                    {

                        if ((access(nam1, F_OK)!=0) && (access(nam2, F_OK)!=0))
                        {
                            sp2=false;
                        }
                    }

                }

                /////////////////////////////////////////////
                unlink(myfifo);
                unlink(myfifo2);
                ///////////////////////////////////////////////////////////////
                //printf("%s has secret %lu\n",service_name,secret);
                printf("%s and %s have exchanged their secrets\n",array[1],array[2]);


            }

        }
////////////////////////////////////////////////////////////////////////////////////////////////
    }else
    {
        ////////////////////////
        //read input from parent

        int ind2 = 0;
        //////////////////////////
        int num2=0;

        ///////////////
        int rd2=-1;
        while(1)
        {

            char command2[128];
            char* token2;
            char* rest2;

            char at1[128];
            char at2[128];
            char at3[128];

            char message2[128];

            char reply2[128];
            //char* tag2;
            char tag2[3];




            ////////////////////////////////////////////////
            strcpy(command2, readFromParent());


            //if (strstr(command2, "exchange") != NULL)
            //{
            //    char tmp2[128];
            //    strcpy(tmp2, command2);
            //    if(strlen(command2)<12)
            //    {
            //        strcat(tmp2,readFromParent());
            //    }

            //    tmp2[strlen(tmp2)]='\0';
            //    strcpy(command2, tmp2);
            //}
            /////////////////////////////////////////////
            //printf("dddd %s\n",command2);



            strcpy(message2, command2);
            rest2=command2;


            ind2=0;

            char *array2[4];

            while ((token2 = strtok_r(rest2, " ", &rest2)))
            {
                array2[ind2] = token2;
                ind2++;
            }




            /////////////////////////////////


            //////////////////////////////
            ///////////////////////////////////////
            if(strcmp(array2[0], "spawn") == 0)
            {
                printf("%s has received %s\n",service_name,message2);
                /////////////////////////////////////////////////////////////
                //printf("%s\n","spawn service");
                strcpy(reply2, ansN);
                rd2=-1;
                //strcpy(TEMP,"a");

                if(strcmp(array2[1], service_name) == 0)
                {

                    MySpawn(array2[2]);
                    childrenNum++;

                    /////////////////////////////////////
                    //SPAWN METHOD2:
                    Node* current = head;
                    while (current!= NULL)
                    {

                        Service* ser2 = current->service;

                        if(strcmp(ser2->name,array2[2]) == 0)
                        {
                            rd2=ser2->read_fd;
                            break;
                        }
                        current = current->next;
                    }



                    strcpy(reply2,readFomChild(rd2));
                    /////////////////////////////////////////
                    reply2[strlen(reply2)]='\0';
                    ///////////////////////////////
                    writeToParent(reply2);

                }else
                {

                    /////////////////////////
                    message2[strlen(message2)]='\0';
                    //if already Manager->A , and you type spawn B C,

                    Node* current = head;

                    //printf("%s\n", (head->service)->name);
                    while (current!= NULL)
                    {

                        Service* ser2 = current->service;


                        //printf("haaaaaaaaa\n");
                        writeToChild(ser2->write_fd, message2);

                        strcpy(reply2, readFomChild(ser2->read_fd));
                        //printf("kk%skk",reply2);
                        if(strcmp(reply2,ansY) == 0)
                        {
                            break;
                        }
                        current = current->next;
                    }

                    reply2[strlen(reply2)]='\0';
                    writeToParent(reply2);
                }
                /////////////////////////////////////////////////


            }else if(strcmp(array2[0], "kill") == 0)
            {

                //tag2 = array2[2];

                strcpy(tag2, array2[2]);


                num2=0;
                //////////////////////////

                if(strcmp(tag2, "N") == 0)
                {


                    strcpy(at1, concat(array2[0], " "));
                    strcpy(at2, concat(at1,array2[1]));

                    at2[strlen(at2)]='\0';

                    printf("%s has received %s\n",service_name,at2);


                }
                ///////////////////////////////

            /////////////////////////////////////////////////////////////////////

                ////////////////////////////////////////////////////////
                //reply2="0";
                strcpy(reply2, "0");
                if(strcmp(array2[1], service_name) == 0)
                {

                    ///////////////
                    strcpy(at1, concat(array2[0], " "));
                    strcpy(at2, concat(at1,array2[1]));
                    strcpy(message2, concat(at2," Y"));

                    message2[strlen(message2)]='\0';


                    Node* current = head;
                    while (current!= NULL)
                    {

                        Service* ser2 = current->service;


                        writeToChild(ser2->write_fd, message2);
                        //reply2=readFomChild(ser2->read_fd);
                        strcpy(reply2, readFomChild(ser2->read_fd));

                        num2+=atoi(reply2);

                        int status;
                        waitpid(ser2->pid, &status, 0);

                        current = current->next;
                    }

                    /////////////////////////////////
                    num2+=1;
                    ////////////////////////
                    writeToParent(intTostr(num2));

                    die();

                }else
                {
                    message2[strlen(message2)]='\0';
                    if(strcmp(tag2, "N") == 0)
                    {

                        Node* current = head;
                        while (current!= NULL)
                        {

                            Service* ser2 = current->service;

                            writeToChild(ser2->write_fd, message2);

                            //reply2=readFomChild(ser2->read_fd);
                            strcpy(reply2, readFomChild(ser2->read_fd));

                            num2+=atoi(reply2);
                            if(strcmp(ser2->name,array2[1]) == 0)
                            {
                                int status;
                                waitpid(ser2->pid, &status, 0);

                                deleteNode(ser2->name);
                            }

                            if(num2>0)
                            {
                                break;
                            }


                            current = current->next;
                        }
                        //////////////////////////////
                        writeToParent(intTostr(num2));
                        /////////////////////////////////////

                    }else
                    {
                        ///////////////////////////////////
                        Node* current = head;
                        while (current!= NULL)
                        {

                            Service* ser2 = current->service;

                            writeToChild(ser2->write_fd, message2);

                            strcpy(reply2, readFomChild(ser2->read_fd));
                            //reply2=readFomChild(ser2->read_fd);

                            num2+=atoi(reply2);
                            //////////////////////////
                            int status;
                            waitpid(ser2->pid, &status, 0);
                            /////////////////////////////////

                            current = current->next;
                        }
                        /////////////////////////////////////////

                        num2+=1;
                        writeToParent(intTostr(num2));

                        ////////////////////////////////////
                        die();
                    }

                    ////////////////////////////////////////
                }
                /////////////////


            }else if(strcmp(array2[0], "exchange") == 0)
            {


                //////////////////////////////////////////////

                char w1[128];
                char w2[128];
                char w3[128];
                char w4[128];

                strcpy(w1, concat(array2[0], " "));
                strcpy(w2, concat(w1,array2[1]));
                strcpy(w3, concat(w2," "));
                strcpy(w4, concat(w3,array2[2]));


                w4[strlen(w4)]='\0';

                printf("%s has received %s\n",service_name,w4);
                //////////////////////////////////////////////

                //printf("%s original secret: %lu\n",service_name ,secret);


                int gnum2=0;
                ////////////////////////////////////////////////////////////////
                if(strcmp(service_name, array2[1]) == 0)
                {

                    char nam1[128];
                    char nam2[128];

                    strcpy(nam1, concat(array2[1],".txt"));
                    nam1[strlen(nam1)]='\0';

                    strcpy(nam2, concat(array2[2],".txt"));
                    nam2[strlen(nam2)]='\0';

                    ////////////////////////////////////////////////
                    FILE *fp1 = NULL;
                    fp1 = fopen(nam1 ,"a");
                    ///////////////////////////////////////////
                    char c1[128];
                    char c2[128];

                    char m1[128];
                    char m2[128];

                    strcpy(c1, concat(array2[1],"_to_"));
                    strcpy(c2, concat(c1,array2[2]));

                    strcpy(m1, concat(array2[2],"_to_"));
                    strcpy(m2, concat(m1,array2[1]));

                    char * myfifo=c2; //= "AToB";
                    char * myfifo2=m2; //= "BToA";
                    ///////////////////////////////////////////

                    ////////////////////////////////////////////////
                    gnum2=atoi(array2[3]);
                    gnum2+=1;

                    if(gnum2==2)
                    {
                        //gnum: you are the second target
                        writeToParent(intTostr(gnum2));
                        ////////////////////////////////////////////////////////
                        char tm[128];
                        char tp[128];
                        //start exchange
                        strcpy(tp,intTostr(secret));

                        tp[strlen(tp)]='\0';
                        int fd = open(myfifo, O_WRONLY);
                        //////////////////////////////////////
                        write(fd, tp, strlen(tp)+1);//
                        close(fd);
                        ///////////////////////////////////
                        int fd2 = open(myfifo2, O_RDONLY);
                        read(fd2, tm, sizeof(tm));
                        close(fd2);


                        secret =atoi(tm);
                        //printf("%s after seccret: %lu\n",service_name,secret);
                        printf("%s has acquired a new secret from %s, value: %lu\n",array2[1],array2[2],secret);




                    }else
                    {

                        //gnum: you are the first target

                        char k1[128];
                        char k2[128];
                        char k3[128];
                        char k4[128];
                        char k5[128];

                        strcpy(k1, concat(array2[0], " "));
                        strcpy(k2, concat(k1,array2[1]));
                        strcpy(k3, concat(k2," "));
                        strcpy(k4, concat(k3,array2[2]));
                        strcpy(k5, concat(k4," "));
                        strcpy(message2, concat(k5,intTostr(gnum2)));

                        message2[strlen(message2)]='\0';


                        //////////////////////////////////////////
                        Node* current = head;
                        while (current!= NULL)
                        {
                            Service* ser2 = current->service;
                            writeToChild(ser2->write_fd, message2);
                            strcpy(reply2, readFomChild(ser2->read_fd));

                            if(atoi(reply2)==2)
                            {
                                gnum2=atoi(reply2);
                                break;
                            }

                            current = current->next;
                        }



                        if(gnum2==2)
                        {
                            //read first , then write
                            char tm[128];
                            char tp[128];

                            //start exchange
                            int fd2 = open(myfifo2, O_RDONLY);
                            ////////////////////////////////////////
                            read(fd2, tm, sizeof(tm));
                            close(fd2);
                            /////////////////////////////////////////

                            strcpy(tp,intTostr(secret));
                            tp[strlen(tp)]='\0';

                            int fd = open(myfifo, O_WRONLY);
                            write(fd, tp, strlen(tp)+1);
                            close(fd);
                            /////////////////////////////////////////////

                            secret =atoi(tm);
                            //printf("%s after seccret: %lu\n",service_name,secret);
                            printf("%s has acquired a new secret from %s, value: %lu\n",array2[1],array2[2],secret);
                            writeToParent(intTostr(gnum2));




                        }else
                        {

                            char tm[128];
                            char tp[128];
                            //no target in your descendant or papa
                            writeToParent(intTostr(gnum2));
                            bool sp=true;

                            //wait for the other side
                            while(sp)
                            {

                                if ((access(nam1, F_OK)==0) && (access(nam2, F_OK)==0))
                                {
                                    //both file exists
                                    sp=false;
                                }
                            }

                            //////////////////////////////////////
                            //first read then write
                            //start exchange
                            int fd2 = open(myfifo2, O_RDONLY);
                            ////////////////////////////////////////
                            read(fd2, tm, sizeof(tm));
                            close(fd2);
                            /////////////////////////////////////////
                            strcpy(tp,intTostr(secret));
                            tp[strlen(tp)]='\0';

                            //////////////////////////////
                            //printf("%s send %s\n",service_name,tp);

                            int fd = open(myfifo, O_WRONLY);
                            write(fd, tp, strlen(tp)+1);
                            close(fd);
                            /////////////////////////////////////////////
                            secret =atoi(tm);
                            printf("%s has acquired a new secret from %s, value: %lu\n",array2[1],array2[2],secret);
                            //printf("%s after seccret: %lu\n",service_name,secret);

                            /////////////////////////////////////

                        }
                    }

                    ////////////////////////////////////////////////
                    fclose(fp1);
                    unlink(nam1);
                    //////////////////
                /////////////////////////////////////////////////////////////////////
                }else if(strcmp(service_name, array2[2]) == 0)
                {

                    ////////////////////////////////////////////////////////////////
                    //char first1[128];//
                    //char first2[128];
                    ///////////////////////////////////////////////////////////////////
                    //printf("original secret: %lu\n", secret);
                    char nam1[128];
                    char nam2[128];

                    strcpy(nam1, concat(array2[1],".txt"));
                    nam1[strlen(nam1)]='\0';

                    strcpy(nam2, concat(array2[2],".txt"));
                    nam2[strlen(nam2)]='\0';

                    ////////////////////////////////////////////////
                    FILE *fp2 = NULL;
                    fp2 = fopen(nam2 ,"a");
                    ///////////////////////////////////////////
                    char c1[128];
                    char c2[128];

                    char m1[128];
                    char m2[128];

                    strcpy(c1, concat(array2[1],"_to_"));
                    strcpy(c2, concat(c1,array2[2]));

                    strcpy(m1, concat(array2[2],"_to_"));
                    strcpy(m2, concat(m1,array2[1]));

                    char * myfifo=c2; //= "AToB";
                    char * myfifo2=m2; //= "BToA";
                    ///////////////////////////////////////////

                    ////////////////////////////////////////////////
                    gnum2=atoi(array2[3]);
                    gnum2+=1;

                    if(gnum2==2)
                    {
                        //gnum: you are the second target

                        writeToParent(intTostr(gnum2));
                        ////////////////////////////////////////////////////////
                        char tm[128];
                        char tp[128];
                        //start exchange
                        strcpy(tp,intTostr(secret));

                        tp[strlen(tp)]='\0';
                        int fd = open(myfifo2, O_WRONLY);
                        //////////////////////////////////////
                        write(fd, tp, strlen(tp)+1);
                        close(fd);
                        //////////////////////////////////
                        int fd2 = open(myfifo, O_RDONLY);
                        read(fd2, tm, sizeof(tm));
                        close(fd2);

                        //printf("%s receive %s\n",service_name,tm);


                        secret =atoi(tm);
                        //printf("%s after seccret: %lu\n",service_name,secret);
                        printf("%s has acquired a new secret from %s, value: %lu\n",array2[2],array2[1],secret);


                    }else
                    {

                        //gnum: you are the first target

                        char k1[128];
                        char k2[128];
                        char k3[128];
                        char k4[128];
                        char k5[128];

                        strcpy(k1, concat(array2[0], " "));
                        strcpy(k2, concat(k1,array2[1]));
                        strcpy(k3, concat(k2," "));
                        strcpy(k4, concat(k3,array2[2]));
                        strcpy(k5, concat(k4," "));
                        strcpy(message2, concat(k5,intTostr(gnum2)));

                        message2[strlen(message2)]='\0';


                        //////////////////////////////////////////
                        Node* current = head;
                        while (current!= NULL)
                        {
                            Service* ser2 = current->service;
                            writeToChild(ser2->write_fd, message2);
                            strcpy(reply2, readFomChild(ser2->read_fd));

                            if(atoi(reply2)==2)
                            {
                                gnum2=atoi(reply2);
                                break;
                            }

                            current = current->next;
                        }



                        if(gnum2==2)
                        {
                            //read first , then write
                            char tm[128];
                            char tp[128];

                            //start exchange
                            int fd2 = open(myfifo, O_RDONLY);
                            ////////////////////////////////////////
                            read(fd2, tm, sizeof(tm));
                            close(fd2);
                            /////////////////////////////////////////

                            strcpy(tp,intTostr(secret));
                            tp[strlen(tp)]='\0';

                            int fd = open(myfifo2, O_WRONLY);
                            write(fd, tp, strlen(tp)+1);
                            close(fd);
                            /////////////////////////////////////////////

                            secret =atoi(tm);
                            //printf("%s after seccret: %lu\n",service_name,secret);
                            printf("%s has acquired a new secret from %s, value: %lu\n",array2[2],array2[1],secret);
                            writeToParent(intTostr(gnum2));



                        }else
                        {

                            char tm[128];
                            char tp[128];
                            //no target in your descendant or papa
                            writeToParent(intTostr(gnum2));
                            bool sp=true;

                            //wait for the other side
                            while(sp)
                            {

                                if ((access(nam1, F_OK)==0) && (access(nam2, F_OK)==0))
                                {
                                    //both file exists
                                    sp=false;
                                }
                            }

                            //////////////////////////////////////
                            //first read then write
                            //start exchange
                            int fd2 = open(myfifo, O_RDONLY);
                            ////////////////////////////////////////
                            read(fd2, tm, sizeof(tm));
                            close(fd2);
                            /////////////////////////////////////////
                            strcpy(tp,intTostr(secret));
                            tp[strlen(tp)]='\0';

                            //////////////////////////////



                            int fd = open(myfifo2, O_WRONLY);
                            write(fd, tp, strlen(tp)+1);
                            close(fd);
                            /////////////////////////////////////////////
                            secret =atoi(tm);
                            printf("%s has acquired a new secret from %s, value: %lu\n",array2[2],array2[1],secret);
                            //printf("%s after seccret: %lu\n",service_name,secret);

                            /////////////////////////////////////

                        }
                    }

                    ////////////////////////////////////////////////
                    fclose(fp2);
                    unlink(nam2);
                    //////////////////

                /////////////////////////////////////////////////////////////////////
                }else
                {


                    char k1[128];
                    char k2[128];
                    char k3[128];
                    char k4[128];
                    char k5[128];

                    //printf("hihi\n");

                    //for(int j=0;j<4;j++)
                    //{
                    //     printf("%s\n",array2[j]);
                    //}


                    gnum2=atoi(array2[3]);



                    strcpy(k1, concat(array2[0], " "));
                    strcpy(k2, concat(k1,array2[1]));
                    strcpy(k3, concat(k2," "));
                    strcpy(k4, concat(k3,array2[2]));
                    strcpy(k5, concat(k4," "));




                    Node* current = head;
                    while (current!= NULL)
                    {

                        strcpy(message2, concat(k5,intTostr(gnum2)));
                        message2[strlen(message2)]='\0';

                        Service* ser2 = current->service;
                        writeToChild(ser2->write_fd, message2);
                        strcpy(reply2, readFomChild(ser2->read_fd));
                        gnum2=atoi(reply2);

                        if(gnum2==2)
                        {
                                break;
                        }

                        current = current->next;
                    }

                    writeToParent(intTostr(gnum2));

                }
                ///////////////////////////////////

        ///////////////////////////////////////////////////////////////////////
                //printf("%s\n","exchange service");
            }


        }


    }
///////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////
    return 0;
}
