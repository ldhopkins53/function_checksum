#include <stdio.h>
#include <stdlib.h>

extern void *main_function_size;
int main_function_crc __attribute__((section(".main_function_crc"))) = 0;

int __attribute((section(".main_function_section"))) main()
{
    printf("testing this one out\n");
    return EXIT_SUCCESS;
}
