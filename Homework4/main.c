//
//  main.c
//  Homework4
//
//  Created by jr2339 on 3/27/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
int fd;

typedef struct phone_book_record {
    char name[80];
    char number[10];
}PhoneBookRecord;


void add_record(PhoneBookRecord* a_record);
int get_record(char* number, PhoneBookRecord* returned_record);
int remove_record(char* number);
void printDB();

int fd;
char *filename;

int main(int argc, const char * argv[]) {
    if(argc < 2){
        // Insufficient arguments
        fprintf(stderr, "usage: %s FLIE",argv[0]);
        exit(EXIT_FAILURE);
    }
    filename = malloc(80*sizeof(char));
    strcpy(filename, argv[1]);
    fd = open(filename,O_RDWR|O_CREAT,0644);
    if (fd==-1) {
        fprintf(stderr, "%s: Couldn't open file %s, %s\n",argv[0],argv[1],strerror(errno));
        exit(EXIT_FAILURE);
    }
    PhoneBookRecord pbr1 = {"John", "123456"};
    PhoneBookRecord pbr2 = {"Joun", "234567"};
    PhoneBookRecord pbr3 = {"Zach", "232323"};
    PhoneBookRecord pbr4 = {"Simon", "86004"};
    PhoneBookRecord pbr5 = {"Jun", "86001"};
    add_record(&pbr1);
    printDB();
    printf("================\n");
    add_record(&pbr2);
    printDB();
    printf("================\n");
    add_record(&pbr3);
    printDB();
    printf("================\n");
    add_record(&pbr4);
    printDB();
    printf("================\n");
    add_record(&pbr5);
    printDB();
    printf("================\n");
    get_record("86001", &pbr5);
    printf("\n");
    printf("================\n");
    remove_record("234567");
    remove_record("123456");
    

    printDB();

    return EXIT_SUCCESS;
}

void add_record(PhoneBookRecord* a_record){
    char buff[sizeof(PhoneBookRecord)];
    lseek(fd, 0, SEEK_SET);
    read(fd, buff, sizeof(PhoneBookRecord));
    lseek(fd,0,SEEK_END);
    write(fd,buff,sizeof(PhoneBookRecord));
    lseek(fd,0,SEEK_SET);
    write(fd,a_record,sizeof(PhoneBookRecord));
 
}

int get_record(char* number, PhoneBookRecord* returned_record){  // the the first parameter shoule be name
    int result =0;
    int position =0;
    size_t rr ;
    // Search from the begining of file
    lseek(fd, 0, SEEK_SET);
    
    while((rr= read(fd, returned_record,sizeof(PhoneBookRecord)))>0){
        position++;
        if (strcmp(number, returned_record->number)==0) {
            printf("%s:%s\n",returned_record->name,returned_record->number);
            printf("We get this record, the position is %d",position);
            break;
        }
        else{
            result = -1;
        }
    }
    if(rr==-1){
        perror(NULL);
    }
    return result;
}


int remove_record(char* number){
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
        truncate(filename, phone_book_size-sizeof( PhoneBookRecord));
    }
    return find;
}

void printDB(){
    PhoneBookRecord phone_book;
    size_t p_b ;
    lseek(fd, 0, SEEK_SET);
    while ((p_b = read(fd, &phone_book,sizeof(phone_book))) > 0) {
        printf("%s:%s\n",phone_book.name,phone_book.number);
    }
}















