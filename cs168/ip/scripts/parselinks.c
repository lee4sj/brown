#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "parselinks.h"

#include "util/dbg.h"

int parse_triple(FILE *f, char phys_host[HOST_MAX_LENGTH], uint16_t *phys_port,
                 struct in_addr *virt_ip){
  char phys_host_in[256];
  int phys_port_in;
  int virt_ip_in[4];
  int i;

  int ret = fscanf(f, "%255[^:]:%d %d.%d.%d.%d ", phys_host_in, &phys_port_in,
                   virt_ip_in, virt_ip_in+1, virt_ip_in+2, virt_ip_in+3);
  if (ret != 6)
    return -1;

  (void) strcpy(phys_host, phys_host_in);

  if (phys_port_in < 0x0000 || phys_port_in > 0xffff)
    return -1;
  *phys_port = phys_port_in;

  virt_ip->s_addr = 0;
  for (i=0; i<4; i++){
    if (virt_ip_in[i] < 0 || virt_ip_in[i] > 255)
      return -1;
    virt_ip->s_addr |= virt_ip_in[i] << (24-i*8);
  }
  return 0;
}

int parse_line(FILE *f, lnxline_t *link){
  int ret = parse_triple(f, link->local_phys_host, &link->local_phys_port,
                         &link->local_virt_ip);
  if (ret == -1)
    return -1;
  ret = parse_triple(f, link->remote_phys_host, &link->remote_phys_port,
                     &link->remote_virt_ip);
  if (ret == -1)
    return -1;

  return 0;
}

int count_links(FILE *f){
  int c;
  int lines = 0;
  while ((c = fgetc(f)) != EOF){
    if (c == '\n')
      lines++;
  }
  return lines;
}

list_t *parse_links(char *filename){
  int i, ret;
  FILE *f;
  list_t *links;
  lnxline_t *line;
  f = fopen(filename,"r");
  if (f == NULL){
    dbg(DBG_ERROR, "parse_links: unable to open \"%s\"\n", filename);
    return NULL;
  }

  links = (list_t *)malloc(sizeof(list_t));
  list_init(links);

  while (!feof(f)) {
    line = (lnxline_t *)malloc(sizeof(lnxline_t));
    if (line == NULL){
      fprintf(stderr, "parse_links: out of memory\n");
      exit(1);
    }

    ret = parse_line(f, line);
    if (ret == -1){
      dbg(DBG_ERROR, "parse_links: error parsing line %d of \"%s\", returning what's available.\n", i, filename);
      free(line);
      return links;
    }

    list_insert_tail(links, &line->listnode);
  }
  ret = fclose(f);
  if (ret == -1){
    dbg(DBG_ERROR, "parse_links: unable to close \"%s\"\n", filename);
  }
  return links;
}

void free_links(list_t *links){
  lnxline_t *line;

  //Two methods to iterate through the list
  //if you only care about the variable, do this:
  list_iterate_begin(links, line, lnxline_t, listnode) {    
    free(line);
  } list_iterate_end();

  //if you care about the links and want to list_remove, list_insert, etc:
  /*
  list_link_t *curlink;
  for (curlink = links->l_next; curlink != links; curlink = curlink->l_next)
  {
    //listnode is the member of lnxline_t that is a list_link_t
    line = list_item(curlink, lnxline_t, listnode);
    free(line);
  }
  */
  
  free(links);
}
