/*
 * Utils.h
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#ifndef APP_UTILS_H_
#define APP_UTILS_H_

#include <cstring>

#include "openssl/conf.h"
#include "openssl/evp.h"
#include "openssl/err.h"

#include "user_type.h"

int enc_aes_gcm(unsigned char *plaintext, int plaintext_len,
                unsigned char *key,
                unsigned char *ciphertext);

int dec_aes_gcm(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *key,
                unsigned char *plaintext);

#endif /* APP_UTILS_H_ */
