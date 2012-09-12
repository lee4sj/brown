#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 4096
#define BASE 62


int index_of(char c, char *str) {
  int rtn = 0;

  while (*str != '\0') {
    if (c == *str) {
      return rtn;
    }

    str++;
    rtn++;
  }

  return -1;
}

char *encode_ip_packet(char *packet, unsigned int len) {
  char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *encoded_vals = (char *)malloc(MAX_LEN);
  memset(encoded_vals, 0, MAX_LEN);

  int bytes_encoded = 0;
  int chars_written = 0;
  int e_indx = 0;

  while (bytes_encoded < len) {

    //printf("bytes_encoded: %d\n", bytes_encoded);

    if (chars_written == 62) {
      encoded_vals[e_indx] = '.';
      chars_written = 0;
      e_indx++;
      continue;
    }

    unsigned char current_byte = *(packet + bytes_encoded);

    unsigned char temp_buffer[2];
    memset(temp_buffer, '0', 2);
    int temp_index = 0;

    if (current_byte == 0) {
      encoded_vals[e_indx] = '0';
      e_indx++;
      encoded_vals[e_indx] = '0';
      e_indx++;
      bytes_encoded++;
      chars_written += 2;
      continue;
    }

    while (current_byte > 0) {
      char remainder = current_byte % BASE;
      current_byte = current_byte / BASE;
      temp_buffer[temp_index] = alphabet[remainder];
      temp_index++;
    }

    // swap
    unsigned char temp = temp_buffer[0];
    temp_buffer[0] = temp_buffer[1];
    temp_buffer[1] = temp;

    // insert
    encoded_vals[e_indx] = temp_buffer[0];
    e_indx++;
    encoded_vals[e_indx] = temp_buffer[1];
    e_indx++;

    bytes_encoded++;
    chars_written += 2;
  }

  encoded_vals[e_indx] = '\0';
  return encoded_vals;
}


char *decode_str(char *text, unsigned int *out_len) {
  char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  unsigned char *decoded_vals = (char *)malloc(MAX_LEN);
  memset(decoded_vals, 0, MAX_LEN);

  int bytes_decoded = 0;
  int txt_indx = 0;
  int d_indx = 0;

  while (1) {
    if (*(text + txt_indx) == '\0') {
      break;
    }

    if (*(text + txt_indx) == '.') {
      txt_indx++;
      continue;
    }

    char first_char = *(text + txt_indx);
    char second_char = *(text + txt_indx + 1);

    unsigned char current_byte = 0;
    current_byte += BASE * index_of(first_char, alphabet);
    current_byte += index_of(second_char, alphabet);
    txt_indx += 2;

    decoded_vals[d_indx] = current_byte;
    bytes_decoded++;
    d_indx++;
  }

  *out_len = bytes_decoded;
  return decoded_vals;
}


/*
int main() {
  char testin[100];
  int i;

  for (i = 0; i < 100; i++) {
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    char c = (char)((tv.tv_nsec) % 256);
    testin[i] = c;
    printf("testin[%d] = %hhu\n", i, c);
  }

  printf("\n");

  char *result = encode_ip_packet(testin, 100);

  printf("result: %s\n", result);

  int x;
  char *testout = decode_str(result, &x);
  for (i = 0; i < x; i++) {
    printf("testout[%d] = %hhu\n", i, testout[i]);
  }
  printf("\n");

  free(result);
  return 0;
}
*/
