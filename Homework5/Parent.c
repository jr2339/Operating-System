//
//  main.c
//  Homework5
//
//  Created by Jun Rao and Mitchell Hewitt
//  Copyright © 2016 jr2339. All rights reserved.
//


// Used for wait functions.
#include <sys/wait.h>
#include <stdio.h>
// Used for the memory allocation calls.
#include <stdlib.h>
// Used for the system calls for file control (e.g. open(), read(), etc.)
#include <unistd.h>
// Used for the definitions of file operations (e.g. O_TRUNC)
#include <fcntl.h>
// Used for the definitions of permissions (e.g. S_IRUSR)
#include <sys/stat.h>
// Used for strcmp().
#include <string.h>

typedef struct phone_book_record {
    char name[80];
    char number[10];
}PhoneBookRecord;

void printDB();
int fd;


int main(int argc, const char * argv[]) {
    pid_t child_pid;//
    char* name = "Jun";
    char buffer[10];
    pid_t childarray[10];
    int i;
    fd = open("input.txt",O_RDWR|O_CREAT,0644);
    for(i=0;i<10;i++){                              //make 10 children
        printf("Now, We will start Fork %d\n",i+1);
        child_pid = fork();                         //fork the parent to create a child
        childarray[i] = child_pid;                  //store child's process id at corresponding position of childarray
        if(child_pid==0){ //This is the child process
            sprintf(buffer, "%d", getpid());        //changes the process id integer to string
                printf("Child: my pid is %d, parent pid is %d\n",getpid(), getppid());
                execl("./Child","./Child",name,buffer,NULL);    //passes the name and number into an execution of the child file
        }
        
       else if(child_pid<0){ // We're the parent process, but child couldn't be created
            perror("Fork failed!");
            exit(-1);
       }

    }
    
    int return_from_wait;
    //otte's code
    while(i--){ //automatically decrements child array's index
        return_from_wait = wait(&childarray[i]); //wait until the corresponding child has terminated
        if(return_from_wait == -1){
            i++;
        }
        else{
            printf("Process %d returned.\n", return_from_wait);         //inform the user that the child terminated
            printf("==============================================\n");
        }
    }
    
    printDB();          //print out the final resulting database
    close(fd);          //close the file
    return 0;
        
}


void printDB(){
    PhoneBookRecord phone_book;
    lseek(fd,0,SEEK_SET);
    while(read(fd,&phone_book,sizeof(phone_book))>0){
        printf("%s:%s\n",phone_book.name,phone_book.number);
    }
}