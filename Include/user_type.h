/*
 * user_type.h
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#ifndef INCLUDE_USER_TYPE_H_
#define INCLUDE_USER_TYPE_H_

#define AESGCM_IV_SIZE 12
static unsigned char gcm_iv[] = {
    0x99, 0xaa, 0x3e, 0x68, 0xed, 0x81, 0x73, 0xa0, 0xee, 0xd0, 0x66, 0x84
};

#define AESGCM_MAC_SIZE 16

#define ENC_KEY_SIZE 16 //16 bytes for AES128

#define USER_NAME_SIZE 20 // size of username

#define RAND_LEN 256
typedef struct
{
    size_t content_length;
    unsigned char content[RAND_LEN];
} rand_t;

#endif /* INCLUDE_USER_TYPE_H_ */
