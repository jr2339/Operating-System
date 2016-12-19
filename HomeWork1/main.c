//
//  main.c
//  HomeWork1
//
//  Created by jr2339 on 2/3/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

/*
 Our string is looks like a 2-D Array
 Strings = [
              [His name is ]   // the longest sub-array which size is 13
              [Jun.]
              [He is very ]
              [Cool.]
              ['\0']
            ]
 This is a 5*13 2-D string array. Because we have 5 row
 And the longest string size is 12 which include space and punctuation
 We have two loops.
 Out side loop is iterate in the row, which means check the first sub-array, second-subarray, whoes pointer is Strings.
 Inside loop is check the elements in each sub-array, whose pointer is *Strings.
 By the way, the last sub-array ['\0'] just tell the outside pointer to stop iterate at here.
 
 After concatenate, the answer should be "His name is Jun.He is very Cool."
 
 */

#include <stdio.h>
#include <stdlib.h>

char* conscat_strings(char** strings){
    /* Because the address of Strings[0][0]
             = the addrss of Strings[0]
             = the address of Strings
    */
    
    char* read = (char*)malloc(100*sizeof(char)); // we will reserved 100 bytes space for output string
    char* copy = read;                           // the pointer copy at here, copy the value to our reserved space
    /* 
              Result =[  |   |   |   |   |   |   |..............|]
                  read/\copy
     at very begining, pointer read and pointer copy, point to the same address whcih is the first element in our result array
     Our first input string is "His name is"
     
     Result =[ H |   |   |   |   |   |   |..............|]
          read/\copy
     
     Result =[ H | i  |   |   |   |   |   |..............|]
             /     \
          read     copy
     ......
     ......
     
     At last, it looks like
     
     Result =[ H | i  | s |   | n | a | m |..............|'\0'|]
              /                                              \
            read                                             copy
     Hence, we have two pointers, pointer read for print the result
     Pointer copy: we use *copy which at here is dereference this pointer, 
     let *pointer = ** Strings, to copy the char to our result array
     */
    while (*strings !='\0') {
        while (**strings !='\0') {
            *copy = **strings;
            copy++;
            *strings = *strings +1;
            if (**strings == '\0') {  // if we copy the whole word to copy
                *copy =' ';           // then we add a space at the end of this word
                copy++;               // the pointer of newstring go to next space
            }
        }
        strings++;
    }
 
    printf("%s\n", read);
 
    
    return read;
    
}



int main(int argc, const char * argv[]) {
   
    if(argc <= 2){              // at least we have two strings
        printf("We at least have two input strings!");
        return 1;
    }else{
        conscat_strings((char**)argv+1);
    }
    return 0;
}
