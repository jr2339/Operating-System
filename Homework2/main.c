//
//  main.c
//  Homework2
//
//  Created by jr2339 on 2/11/16.
//  Copyright © 2016 jr2339. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef char* (**Pointer_to_Array)(char*); // Define a new type which is pointer to array
/*
 pt_to_array is a pointer point to array
 if we deference Pointer_to_Array, *Pointer_to_Array, we will get a pointer to function char_double which is pt_to_char_double
 if we deference pt_to_char_double which is **Pointer_to_Array, then we get the function char_double
 if we pass the a variable which is a char*, it will return us a pointer which point to string
 */


char* char_double(char* str);
char* char_shift(char* str);
char* char_flip(char* str);
char** string_central(Pointer_to_Array pt_to_array, char* str);



/* String Central takes just two pointers as arguments:
 The first pointer points to an array containing pointers to functions, i.e. the processors.
 Our array looks like
 Poniter Array[3] = [pt_to_char_double,pt_to_char_double,pt_to_char_double]
 So, I will create a pointer to our array which is pt_to_array
 
 
 */
char** string_central(Pointer_to_Array pt_to_array, char* str){
    /*
     We nned to return a string address, which from we call the function. Before we call our function,
     we need to dereference function pointer which is pt_to_element_in_array.
     (* pt_to_element_in_array)----> pointer to string
     hence at here we need char**
     */
    char** result=(char**)malloc(sizeof(char**)*10);// reserved enough space for our result
    char** copy = result;
    
    while (*pt_to_array) {
        *copy++=(*pt_to_array)(str);
        pt_to_array++;
    }
    *copy = NULL;

    return result;
}

char* char_double(char* str){
    char* read_double = (char*)malloc(50*sizeof(char));
    char* copy = read_double;
    while(*str){
        *copy = *str;
        copy++;
        *copy = *str;
        str++;
        copy++;
        
    }
    if (*str == '\0') {
        *copy ='\0';
        copy++;
    }
    
    return read_double;
}

char* char_shift(char* str){
    char* read_shift = (char*)malloc(50*sizeof(char));
    char* copy = read_shift;
    while(*str){
        *copy = *str;
        copy++;
        *copy = *str+1;
        str++;
        copy++;
        
    }
    if (*str == '\0') {
        *copy ='\0';
        copy++;
    }
    return read_shift;
}

char* char_flip(char* str){
    char* read_flip = (char*)malloc(50*sizeof(char));
    char* copy = read_flip;
    while(*str){
        if (*str > 90) {
            *copy = *str -32;
            copy++;
            str++;
        }else{
            *copy = *str+32;
            copy++;
            str++;
        }
        
    }
    if (*str == '\0') {
        *copy ='\0';
        copy++;
    }
    return read_flip;
}



int main(void) {
    char* str = "Hello";
    char** result;
    Pointer_to_Array pt_to_array = (Pointer_to_Array)malloc(sizeof(void*)*4);
    *pt_to_array++ = char_double;
    *pt_to_array++ = char_shift;
    *pt_to_array++ = char_flip;
    *pt_to_array = NULL;
    pt_to_array -=3;
    
    result = string_central(pt_to_array,str);
    while(*result){
        printf("__%s__\n",*result++);
    }
    return 0;
}







