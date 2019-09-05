/*
 * EnclaveUtils.h
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#ifndef ENCLAVE_ENCLAVEUTILS_H_
#define ENCLAVE_ENCLAVEUTILS_H_

#include <cstring>

#include "sgx_trts.h"
#include "sgx_tcrypto.h"

#include "user_type.h"

void prf_Enc(const void *key,const void *plaintext, size_t plaintext_len, rand_t *value);
void prf_Dec(const void *key,const void *ciphertext, size_t ciphertext_len, rand_t *value);

void enc_aes_gcm(const void *key, const void *plaintext, size_t plaintext_len, void *ciphertext);
void dec_aes_gcm(const void *key, const void *ciphertext, size_t ciphertext_len, void *plaintext, size_t plaintext_len);

#endif /* ENCLAVE_ENCLAVEUTILS_H_ */
