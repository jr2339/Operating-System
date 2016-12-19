//
//  main.c
//  Homework3
//
//  Created by Jun Rao and Mitchell Hewitt on 3/21/16.
//  Copyright © 2016 jr2339. All rights reserved.
//
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#define BUF_SIZE 12

int main(int argc, char ** argv) {
    char temp;
    char buf[BUF_SIZE];
    struct stat sb;
    stat(argv[1],&sb);// linux system call to get file size
    off_t file_size = sb.st_size; // file_size
    int fd_src = open(argv[1], O_RDONLY); // O_RDONLY: Open for reading only
    /*
    How can the variable fd_src represent a file as an integer when passing it to the open method?
    It is a handle that identifies the open file; it is generally called a file descriptor, hence the name is fd
    If we print fd, we will get 3. Input(0), Output(1) and Error(2).
    */
    if (fd_src==-1) {
        //Failed to open source file
        perror("open and read the source");
        return -1;
    }
    
    int fd_dst = open(argv[2],O_CREAT|O_WRONLY);
    
    if (fd_dst ==-1) {
        // Failed to open/create destination file
        perror("open and write destination");
        return -1;
    }
    /*
     I get the file size use the unix sysytem call
     We use system call lseek to read from the end to the beining 
     (file_size/BUF_SIZE) means how many blocks we have
     and we use a for loop to read and write each block
     */
    for (int j = 0; j< (file_size/BUF_SIZE); j++) {
        lseek(fd_src, -1*(j+1)*BUF_SIZE, SEEK_END);
        read(fd_src,buf,BUF_SIZE);
        for (int i=0; i<BUF_SIZE/2; i++) {  //reverse the buffer contents
            temp = buf[i];
            buf[i]= buf[BUF_SIZE-i-1];
            buf[BUF_SIZE-i-1] = temp;
        }
        write(fd_dst, buf, BUF_SIZE);       //write the reversed buffer into destination file
    }
    /*
     if the number of blocks times the BUF_SIZE can't read all of the file, that means we have overflow
     We used the (file_size % BUF_SIZE) to get overflow size and then read from the begining to end of overflow
     and reverse it before writing it to the destination file
     */
    int rest_size =file_size%BUF_SIZE;
    if (rest_size!=0) {
        lseek(fd_src,0, SEEK_SET);
        read(fd_src,buf,rest_size);
        for (int i=0; i<rest_size/2; i++) { //reverse the buffer contents
            temp = buf[i];
            buf[i]= buf[rest_size-i-1];
            buf[rest_size-i-1] = temp;
        }
        write(fd_dst, buf, rest_size);  //write the reverse overflow into the destination file
    }
    close(fd_src);
    close(fd_dst);
    
    return(0);
}
