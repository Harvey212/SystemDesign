#define PARENT_READ_FD 3
#define PARENT_WRITE_FD 4
#define MAX_CHILDREN 8
#define MAX_FIFO_NAME_LEN 64
#define MAX_SERVICE_NAME_LEN 16
#define MAX_CMD_LEN 128
#include <sys/types.h>

/////////////////////////////////////////////////////
typedef struct Service
{
    pid_t pid;
    char name[MAX_SERVICE_NAME_LEN];

    int read_fd;  //for parent to read message from the child  //you are the parent
    int write_fd; //for parent to write message to the child

}Service; //Service;
///////////////////////////

typedef struct Node
{
    struct Service *service;
    struct Node *next;
}Node;
