//
//  main.c
//  Homework5
//
//  Created by Jun Rao and Mitchell Hewitt
//  Copyright © 2016 jr2339. All rights reserved.


//

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
#include <errno.h>

#define USE_LOCK_FILE //presence of this preprocessor directive tells the child to use locks

/*
    We copy the code from our homework4 as a template at here
 */
typedef struct phone_book_record {
    char name[80];
    char number[10];
}PhoneBookRecord;

int fd;

void add_record(PhoneBookRecord* a_record);
int get_record(char* number, PhoneBookRecord* returned_record);
int remove_record(char* number);
void printDB();


int is_locked();
void lock();
void unlock();

//-----------------------------------------------------------------------
/*
 * main()
 */

int main(int argc, char *argv[]) {
    char buffer[10];
    
    fd = open("input.txt",O_RDWR|O_CREAT,0644);
    if (fd==-1) {
        perror("Couldn't open file\n");
        exit(EXIT_FAILURE);
    }

    
    if(argc > 1)
    {

        PhoneBookRecord record;
        strcpy(record.name, argv[1]);
        strcpy(record.number, argv[2]);
        for(int i=0;i<10;i++){                          //add and remove the input record 10 times
            printf("We add %s , %d times\n",argv[2],i+1);
            add_record(&record);
            printf("We remove %s, %d times \n",argv[2],i+1);
            remove_record(argv[2]);
            
        }
            printf("Finally, We add %s\n",argv[2]);
            add_record(&record);                        //add it for good

    }
    close(fd);

    return 0;
}

//creates a marker file.  If one already exists, another child process has locked and we can't enter critical section.
//Once another child process has unlocked, we can now lock and enter this child's critical section.
void lock()
{
#ifdef USE_LOCK_FILE

    while(open("./m.txt",O_CREAT | O_EXCL)<=0);
#endif
}

//Gets rid of the marker file to signify that there is currently no lock.
void unlock()
{
#ifdef USE_LOCK_FILE
    unlink("./m.txt");
#endif
}


void add_record(PhoneBookRecord* a_record){
    lock(); //enter critical section and lock
    char buf[sizeof(PhoneBookRecord)];
    lseek(fd,0,SEEK_SET);
    read(fd,buf,sizeof(PhoneBookRecord)); //copy the first record, put in to the buff
    lseek(fd,0,SEEK_END);
    write(fd,buf,sizeof(PhoneBookRecord)); // add the first at the end of the file
    lseek(fd,0,SEEK_SET);
    write(fd, a_record, sizeof(PhoneBookRecord));// add the record at the begining of the file
    unlock(); //exit critical section and unlock
}

int remove_record(char* number){
    lock(); //enter critical section and lock
    int find = -1;
    PhoneBookRecord *phone_book = malloc(sizeof(PhoneBookRecord));
    size_t phone_book_size = lseek(fd,0,SEEK_END);
    size_t p_b ;
    size_t temp;
    //Search from the begining of file
    lseek(fd, 0, SEEK_SET);
    while ((p_b= read(fd, phone_book,sizeof(PhoneBookRecord)))> 0) {
        if (strcmp(number, phone_book->number)==0) {
            find =0;
        }
        if(find==0){
            temp = read(fd, phone_book,sizeof(PhoneBookRecord));
            if(temp>0){
                lseek(fd,-2*sizeof(PhoneBookRecord),SEEK_CUR);
                write(fd,phone_book,sizeof(PhoneBookRecord));
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
        // Remove entry and shif the rest entries
        lseek(fd, sizeof(PhoneBookRecord), SEEK_CUR);
        truncate("input.txt", phone_book_size-sizeof( PhoneBookRecord));
    }
    unlock();   //exit critical section to unlock
    return find;
}



void printDB(){
    PhoneBookRecord phone_book;
    lseek(fd,0,SEEK_SET);
    while(read(fd,&phone_book,sizeof(phone_book))>0){
        printf("%s:%s\n",phone_book.name,phone_book.number);
    }
}