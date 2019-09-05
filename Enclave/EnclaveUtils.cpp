/*
 * EnclaveUtils.cpp
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#include "EnclaveUtils.h"

void prf_Enc(const void *key,const void *plaintext,size_t plaintext_len, rand_t *value) {
    value->content_length = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + plaintext_len; //important- has to be size_t
	enc_aes_gcm(key, plaintext, plaintext_len, value->content);
}

void prf_Dec(const void *key,const void *ciphertext,size_t ciphertext_len, rand_t *value) {
	value->content_length = ciphertext_len - AESGCM_MAC_SIZE - AESGCM_IV_SIZE;
    dec_aes_gcm(key, ciphertext, ciphertext_len, value->content, value->content_length);
}

void enc_aes_gcm(const void *key, const void *plaintext, size_t plaintext_len, void *ciphertext)
{
  //uint8_t p_dst[ciphertext_len] = {0};

  //p_dst = mac + iv + cipher
	sgx_rijndael128GCM_encrypt(
    (sgx_aes_gcm_128bit_key_t*)key,
		(uint8_t *) plaintext, plaintext_len,
		(uint8_t *) (ciphertext + AESGCM_MAC_SIZE + AESGCM_IV_SIZE), //where  the cipher should be stored
		(uint8_t *)gcm_iv, AESGCM_IV_SIZE,
		NULL, 0,
		(sgx_aes_gcm_128bit_tag_t *) ciphertext);	//the tag should be the first 16 bytes and auto dumped out

  memcpy(ciphertext + AESGCM_MAC_SIZE, gcm_iv, AESGCM_IV_SIZE);

  //copy tag+iv+cipher to ciphertext
  //memcpy(ciphertext, p_dst, ciphertext_len);

  //printf("test\n");
  //print_bytes((uint8_t *) plaintext,(uint32_t) plaintext_len);
}

void dec_aes_gcm(const void *key, const void *ciphertext, size_t ciphertext_len, void *plaintext, size_t plaintext_len){

    //uint8_t p_dst[plaintext_len] = {0};

	sgx_status_t ret = sgx_rijndael128GCM_decrypt(
		(sgx_aes_gcm_128bit_key_t*)key,//,&key, //*WARNING FOR TEXT PURPOSE ONLY-USE KF *********WARNING FOR TEST PURPOSE
		(uint8_t *) (ciphertext + AESGCM_MAC_SIZE + AESGCM_IV_SIZE), plaintext_len,
		(uint8_t *) plaintext,
		(uint8_t *)gcm_iv, AESGCM_IV_SIZE,
		NULL, 0,
		(sgx_aes_gcm_128bit_tag_t *) ciphertext); //get the first 16 bit tag to verify

	//memcpy(plaintext, p_dst, plaintext_len);

  //printf("test\n");
  //print_bytes((uint8_t *) plaintext,(uint32_t) plaintext_len);
}
