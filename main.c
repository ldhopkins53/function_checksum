#include <stdio.h>
#include <stdlib.h>

extern void *main_function_size;
int main_function_crc __attribute__((section(".main_function_crc"))) = 0;

int __attribute__((section(".main_function_section"))) main() {
  printf("testing this one out\n");
  return EXIT_SUCCESS;
}

void __attribute__((constructor)) crc_check() {
  int i;
  int interior_crc = 0;
  char *function_ptr = (char *)main;
  //    for (i = 0; i < (size_t)&main_function_size; ++i)
  for (i = 0; i < 0x1e; ++i) {
    interior_crc += abs(function_ptr[i]);
  }
  printf("Interior CRC: %d\n", interior_crc);
  printf("Precalculated CRC: %d\n", main_function_crc);
  if (interior_crc != main_function_crc) {
    printf("CRC check failed\n");
    exit(EXIT_FAILURE);
  }
}
