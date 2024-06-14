/*
todo:取得 shared memory 的定址空間。
todo:Guess 程式使用 SIGUSR1 信號來通知 Game 程式做數值上的判斷（太大、大小、猜中）。
todo:撰寫 SIGUSR1 handler function，將 shared memory 中的 guess 變數與被猜的數字做比較，並將結果寫回 result 變數當中。
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
typedef struct {
    int guess;
    char result[8];
}data;

data *My_data,*Shm_data;
int retval,shmid;
int ans;
key_t key;

void Check_handler (int signo, siginfo_t *info, void *context)
{   
    if(My_data->guess >ans){
        printf("[game] Guess :%d, smaller \n", My_data->guess);
        strcpy(My_data->result,"smaller"); 
    }else if(My_data->guess < ans){
        printf("[game] Guess :%d, bigger \n", My_data->guess);
        strcpy(My_data->result,"bigger"); 
    }else{
        printf("[game] Guess :%d, bingo \n", My_data->guess);
        strcpy(My_data->result,"bingo"); 
    }
}


void sigint_handler(int signo) {
    /* Detach the share memory segment */
    shmdt(Shm_data);
    /* Destroy the share memory segment */
    printf("\ndestroy the share memory.\n");
    retval = shmctl(shmid, IPC_RMID, NULL);
    if (retval < 0)
    {
        fprintf(stderr, " game.c remove share memory failed\n");
        exit(1);
    }
    exit(0);
}


int main(int argc, char *argv[])
{
    
    if(argc != 3) {
        fprintf(stderr, "Usage: ./game <key> <guess>");
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, sigint_handler);
    struct sigaction Check;
    memset(&Check, 0, sizeof (struct sigaction));
    Check.sa_flags = SA_SIGINFO;
    Check.sa_sigaction =&Check_handler;
    sigaction(SIGUSR1, &Check, NULL);

    key=atoi(argv[1]);
    ans=atoi(argv[2]);


    /* Create the segment */
    if ((shmid = shmget(key, sizeof(My_data), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    /* Now we attach the segment to our data space */
    if ((Shm_data = (data*)shmat(shmid, NULL, 0)) == (data*) -1) {
        perror("shmat");
        exit(1);
    }
    printf(" game.c create and attach the share memory.\n");
    /* Now put some things into the memory for the other process to read */
    My_data=Shm_data;
    printf("[game] Game PID :%d \n", getpid());
    
    while(1);
    
    return 0;

}
