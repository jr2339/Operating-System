//
//  main.c
//  Homework6
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
#include <sys/types.h>
// Used for strcmp()
#include <string.h>
//Used for threads
#include <pthread.h>
#include <errno.h>

typedef struct phone_book_record {
    char name[80];
    char number[20];
}PhoneBookRecord;

//custom structure to hold phonebook record and mutex
typedef struct {
    PhoneBookRecord pbr;
    pthread_mutex_t mutexreference;
}CustomStruct;


//function prototypes
void printDB();
int fd;
void *demo();
void add_record(PhoneBookRecord* a_record);
int remove_record(char* number);


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//Variables of type pthread_mutex_t can also be initialized statically, using the constants PTHREAD_MUTEX_INITIALIZER
int main(int argc, const char * argv[]){
    pthread_t threads[10]; //up to 10 pthreads

    fd = open("input.txt",O_RDWR|O_CREAT,0644);

    //array of custom structs to pass into threads
    CustomStruct structarray[10];
    
    //put a phonebook record into each custom struct
    for(int i=0; i<10; ++i) {
        PhoneBookRecord record;
        strcpy(record.name, "Jun");
        structarray[i].pbr = record;
    }
    //Launch threads
    for(int i=0; i<sizeof(threads)/sizeof(threads[0]); ++i) {
        if(pthread_create(&threads[i],NULL,demo,&structarray[i]) != 0) { //pass in custom structure to each thread
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    //Wait for threads to finish
    for(int i=0; i<sizeof(threads)/sizeof(threads[0]); ++i) {
        if(pthread_join(threads[i],NULL) != 0) {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
    }
    printDB();
    close(fd);          //close the file
    return 0;
}


void* demo(CustomStruct* inputrecord){
    char buffer[20];
    //copy the thread id into the input structure's phonebook record
    sprintf(buffer, "%lu", (unsigned long)pthread_self());
    strcpy(inputrecord->pbr.number,buffer);
    
        for(int i=0;i<10;i++){                          //add and remove the input record 10 times
            printf("We add thread %s , %d times\n",buffer,i+1);
            
            add_record(&inputrecord->pbr);
            printf("We remove thread %s, %d times \n",buffer,i+1);
            remove_record(inputrecord->pbr.number);

        }
        printf("Finally, We add thread %s\n",buffer);
        add_record(&inputrecord->pbr); //add it for good

    return 0;

}

void add_record(PhoneBookRecord* a_record){
    pthread_mutex_lock(&mutex); //enter critical section and lock
    char buf[sizeof(PhoneBookRecord)];
    lseek(fd,0,SEEK_SET);
    read(fd,buf,sizeof(PhoneBookRecord)); //copy the first record, put in to the buff
    lseek(fd,0,SEEK_END);
    write(fd,buf,sizeof(PhoneBookRecord)); // add the first at the end of the file
    lseek(fd,0,SEEK_SET);
    write(fd, a_record, sizeof(PhoneBookRecord));// add the record at the begining of the file
    pthread_mutex_unlock(&mutex);//exit critical section and unlock
}

int remove_record(char* number){
    pthread_mutex_lock(&mutex);//enter critical section and lock
    int find = -1;
    PhoneBookRecord *phone_book = malloc(sizeof(PhoneBookRecord));
    size_t phone_book_size = lseek(fd,0,SEEK_END);
    size_t p_b ;
    size_t temp;
    //Search from the beginning of file
    lseek(fd, 0, SEEK_SET);
    while ((p_b= read(fd, phone_book,sizeof(PhoneBookRecord)))> 0) {
        if (strcmp(number, phone_book->number)==0) {
            find =0;
        }
        if(find==0){// we find the record
            temp = read(fd, phone_book,sizeof(PhoneBookRecord)); //copy the target item to temp
            if(temp>0){
                lseek(fd,-2*sizeof(PhoneBookRecord),SEEK_CUR); // remove temp
                write(fd,phone_book,sizeof(PhoneBookRecord)); // copy the new file to output file
            }
            else{
                break;
            }
        }
    }
    if (p_b==-1) {
        perror(NULL);
    }
    if (p_b==0) {
        printf("%s not found\n",number);
    }
    if (p_b > 0) {
        // Remove entry and shift the rest of entries
        lseek(fd, sizeof(PhoneBookRecord), SEEK_CUR);
        truncate("input.txt", phone_book_size-sizeof( PhoneBookRecord));
    }
    pthread_mutex_unlock(&mutex);  //exit critical section to unlock
    return find;
}


//print out the whole database file
void printDB(){
    PhoneBookRecord phone_book;
    size_t p_b ;
    lseek(fd, 0, SEEK_SET);
    while ((p_b = read(fd, &phone_book,sizeof(phone_book))) > 0) {
        printf("%s:%s\n", phone_book.name,phone_book.number);
    }
}
