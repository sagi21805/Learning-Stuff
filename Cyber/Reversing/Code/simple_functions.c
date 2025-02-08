#include <stdio.h>
#include <stdlib.h>
// Compile with:
// nasm -f win64 simple_functions.asm -o simple_functions.o
// gcc simple_functions.c simple_functions.o
extern char* __strchr(char* src, char wanted);
extern int __strlen(char* src);
extern void* __memcpy(void* dest, const void* src, long long count);

int main() {
    char* a = "Hello World";   
    char* empty = malloc(__strlen(a) + 1);
    empty = __memcpy(empty, "EmptyString", __strlen("EmptyString"));


    printf("Length of \'%s\' is %d\n", a, __strlen(a));
    printf("Character \'r\' is at position %d\n", __strchr(a, 'r') - a);
    printf("Character that is not there is at address %llx\n", __strchr(a, 'c'));
    printf("Changed \'%s\' to", empty);
    printf("\'%s\'", __memcpy(empty, a, __strlen(a)));
    free(empty);
}