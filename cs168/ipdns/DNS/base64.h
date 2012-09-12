#ifndef BASE64_H
#define BASE64_H

/*
 * Given a buffer of size len, returns a new heap allocated 
 * null terminated base64 string.
 */
char *encode_ip_packet(char *packet, unsigned int len);

/*
 * Given a null-terminated base64 string, returns a new heap allocated
 * buffer containing the decoded bytes.  The size of the decoded buffer
 * is written to memory pointed to by out_len.
 */
char *decode_str(char *text, unsigned int *out_len);

#endif
