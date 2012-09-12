#include "dns_builder.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



char *get_txt_payload(char* payload, size_t payload_size, size_t *dest_size){
  /*int size = strlen(payload) / 256;
  if(strlen(payload) % strlen != 0 )
    size++;

  dest = (char *)malloc( size + strlen(payload) );

  char *offset = payload;
  int index = 0;

  
  
  while(strlen(index) > 256){
    dest[
    }*/

  /*
  //  printf("%s\n", payload);
  char *dest = (char*) malloc( payload_size + 2 );
  dest[0] = (unsigned char) payload_size;
  memcpy(dest+1, payload, payload_size);
  dest[strlen(payload) + 1] = '\0';
  *dest_size = payload_size+1;
  */
  //  printf("%s\n", dest);

  unsigned int buf_size = payload_size/255;
  if(payload_size % 255 > 0)
    buf_size++;
  
  buf_size += payload_size + 1;

  char *buf = (char *)malloc(buf_size);

  unsigned int payload_c = 0;
  unsigned int buf_c = 0;

  while(payload_c < payload_size - 1){
    if((payload_size - payload_c) >= 255){
      buf[buf_c] = 255;
      buf_c++;
      memcpy(buf + buf_c, payload + payload_c, 255);
      buf_c += 255;
      payload_c += 255;
    }
    else{
      size_t leftover = strlen(payload + payload_c);
      buf[buf_c] = (char *)leftover;
      buf_c++;
      memcpy(buf + buf_c, payload + payload_c, leftover);
      buf_c += leftover;
      payload_c += leftover;
    }
  }
  
  buf[buf_size - 1] = '\0';
  
  *dest_size = buf_size - 2;
  
  return buf;
}


char *dns_build_txt_packet(uint16_t id, uint16_t flags, uint16_t num_q,
			   uint16_t num_anws, uint16_t num_ns, uint16_t num_addi,
			   char *name,
			   const char *payload, size_t payload_size,
			   int *packet_size){
  struct dns_header header;
  bzero(&header, sizeof(struct dns_header));
  header.id = id;
  uint16_t flag = 0;
  flag |= (1 << 15);
  flag |= (1 << 10);

  header.tags = htons(flag);
  header.num_q = htons(num_q);
  header.num_anws = htons(num_anws);
  header.num_ns = htons(num_ns);
  header.num_addi = htons(num_addi);

  size_t name_size = strlen(name);
  name_size++;

  size_t p_size;
  char *txt_payload;
  if(payload == NULL){
    p_size = 1;
    txt_payload = (char *) malloc(1);
    *txt_payload = 0;
  }
  else
    txt_payload= get_txt_payload(payload, payload_size, &p_size);
  
  struct resrc_rec rr_tag;
  rr_tag.type = htons(0x0010);
  rr_tag.class = htons(0x0001);
  rr_tag.ttl = htonl(0);
  rr_tag.rlength = htons(p_size);

  char *dest = (char *)malloc(sizeof(struct dns_header) + name_size + name_size + 4 + 10 + p_size);

  char *index = dest;
  
  memcpy(dest, &header, sizeof(struct dns_header));
  index += sizeof(struct dns_header);
  
  printf("%s\n", name);

  memcpy(index, name, name_size + 4);
  index += name_size + 4;
  
  memcpy(index, name, name_size);
  index += name_size;
  
  memcpy(index, &rr_tag, 10);
  index += 10;
    
  memcpy(index, txt_payload, p_size);
  index += p_size;
  
  *packet_size = sizeof(struct dns_header) + name_size + name_size + 4 + 10 + p_size ;
  
  free(txt_payload);
  return dest;
}



char *parse_qname(char *qname, int q_len){

  int i = 0;
  int index = 0;
  
  char *dest = (char *)malloc(q_len);
  
  while(qname[i] != '\0'){
    int len = qname[i];
    i++;
    memcpy(&dest[index], &qname[i], len);
    index += len;
    i += len;
    dest[index] = '.';
    index++;
  }
  
  dest[index-1] = '\0';

  return dest;
}

char *form_qname(char *name, int *len) {

  int name_indx = 0;
  int dest_indx = 0;

  char dest[4096];

  while (name[name_indx] != '\0') {
    int cur_len = 0;
    int start_indx = dest_indx;
    dest_indx++;
    /*
    while (name[name_indx] != '.') {
      printf("current char: %c\n", name[name_indx]);
      name_indx++;
      //dest[dest_indx] = name[name_indx];
      //dest_indx++;
      //name_indx++;
      //cur_len++;
      //printf("length now %d\n", cur_len);
      //printf("dest_indx now %d\n", dest_indx);
    }
    */
    return;

    //dest[start_indx] = cur_len;
    //printf("seciton length: set dest[%d] = %d\n", start_indx, cur_len);
    //name_indx++; // advance past the dot
  } 

  //dest[dest_indx] = '\0';
  //dest_indx++;

  //char *rtn = (char *)malloc(dest_indx);
  //memcpy(rtn, dest, dest_indx);
  //*len = dest_indx;
  //return rtn;
}
