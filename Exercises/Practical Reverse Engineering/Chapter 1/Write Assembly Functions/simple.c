#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
// Compile with:
// nasm -f win64 simple_functions.asm -o simple_functions.o
// gcc simple_functions.c simple_functions.o
extern char* __strchr(char* src, char wanted);
extern int __strlen(char* src);
extern void* __memcpy(void* dest, void* src, int count);
extern void* __memset(void* buf, char val, int count);
extern int __strcmp(char* s1, char* s2);
extern char* __strset(char* s, char v);

int main() {

    char* a = "Hello World";   
    printf("Length of \'%s\' is %d\n", a, __strlen(a));
    printf("Character \'r\' is at position %ld\n", __strchr(a, 'r') - a);
    printf("Character that is not there is at address %p\n", __strchr(a, 'c'));
    
    char* empty = malloc(__strlen("EmptyString") + 1);
    printf("Changed \'%s\' to ", empty);
    empty = __memcpy(empty, "EmptyString", (long) __strlen("EmptyString"));
    printf("\'%s\'\n", empty);
    int num = 6;
    empty = __memset(empty, 'a', num);
    printf("Overwrite with %d, \'a\': \'%s\'\n", num, empty);
    printf("\'%s\' is matching \'%s\' %s\n", a, a, __strcmp(a, a) ? "false" : "true");
    printf("\'%s\' is matching \'%s\' %s\n", a, empty, __strcmp(empty, a) ? "false" : "true");
    printf("overwrite \'%s\'", empty);
    printf(" with \'b\' \'%s\'\n", __strset(empty, 'b'));
    free(empty);
}