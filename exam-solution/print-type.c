#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE\n", argv[0]);
    exit(1);
  }

  FILE* f = fopen(argv[1], "rb");

  if (f == NULL) {
    fprintf(stderr, "Cannot open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  unsigned char magic[4];
  if (fread(magic, 1, 4, f) != 4) {
    fprintf(stderr, "Cannot read initial four bytes from %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  if (memcmp(magic, "SCLR", 4) == 0) {
    printf("scalar\n");
  } else if (memcmp(magic, "VDNS", 4) == 0) {
    printf("vector\n");
  } else if (memcmp(magic, "MDNS", 4) == 0) {
    printf("dense matrix\n");
  } else if (memcmp(magic, "MCSR", 4) == 0) {
    printf("matrix in CSR format\n");
  } else {
    printf("Unrecognised header: %02x%02x%02x%02x\n", magic[0], magic[1], magic[2], magic[3]);
  }

  fclose(f);
}
