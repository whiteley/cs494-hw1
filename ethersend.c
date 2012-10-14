#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define CHECKLEN 4
#define MACLEN 6
#define TYPELEN 4
#define MTU 1500
#define FRAMELEN MACLEN + MACLEN + TYPELEN + MTU + CHECKLEN

void usage(char *program_name) {
  // check args
  fprintf(stderr, "usage: %s source destination\n", program_name);
  exit(EXIT_FAILURE);
}

int readmac(char *arg, unsigned char mac[MACLEN]) {
  // read a string like 01:23:45:67:89:ab into 0x0123456789ab
  char *mac_format = "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx";
  return (sscanf(arg, mac_format, &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]) == 6);
}

unsigned framecpy(unsigned char *addr, unsigned char *data, unsigned len) {
  // write into our frame and return the update location
  memcpy(addr, data, len);
  return len;
}

void frame(unsigned char dst[MACLEN], unsigned char src[MACLEN], unsigned count, unsigned char buf[MTU]) {
  // given packet type
  unsigned char packet_type[TYPELEN] = { 0xa, 0xb, 0xc, 0xd };
  // given frame check sequence
  unsigned char checksum[CHECKLEN] = { 0x0, 0x0, 0x0, 0x0 };
  unsigned char frame[FRAMELEN];
  unsigned i, ptr = 0;

  // join frame sections
  ptr += framecpy(frame + ptr, dst, MACLEN);
  ptr += framecpy(frame + ptr, src, MACLEN);
  ptr += framecpy(frame + ptr, packet_type, TYPELEN);
  ptr += framecpy(frame + ptr, buf, count);
  ptr += framecpy(frame + ptr, checksum, CHECKLEN);

  for (i = 0; i < ptr; i++) {
    printf("%02x", frame[i]);
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    usage(argv[0]);
  }

  int count, i, netsum, total = 0;
  register int checksum = 0;

  unsigned char buf[MTU];
  unsigned char dst[MACLEN];
  unsigned char src[MACLEN];

  // parse destination and source mac addresses
  if (readmac(argv[1], dst) != 1 || readmac(argv[2], src) != 1) {
    perror("argument error");
    exit(EXIT_FAILURE);
  }

  while (!feof(stdin)) {
    // attempt to read a full payload
    count = fread(buf, sizeof(char), MTU, stdin);

    // error in input
    if (ferror(stdin)) {
      perror("read error");
      exit(EXIT_FAILURE);
    }

    // build and output frame
    frame(dst, src, count, buf);
    printf("\n");

    // accumulate checksum
    for (i = 0; i < sizeof(buf); i++) {
      checksum += buf[i];
    }

    // accumulate total bytes read
    total += count;
  }

  // convert byte order from checksum register
  netsum = htonl(checksum);
  // copy checksum into buffer
  memcpy(buf, &netsum, CHECKLEN);
  // fill buffer with 0s
  memset(buf + CHECKLEN, 0, MTU - CHECKLEN);
  // build and output checksum frame
  frame(dst, src, MTU, buf);
  printf("\n");

  exit(EXIT_SUCCESS);
}
