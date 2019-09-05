/*
 * Utils.cpp
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#include "Utils.h"

int enc_aes_gcm(unsigned char *plaintext, int plaintext_len,
                unsigned char *key,
                unsigned char *ciphertext)
{
	// MAC + IV + ciphertext
    unsigned char output[AESGCM_MAC_SIZE + AESGCM_IV_SIZE + plaintext_len * 2] = {0};
    memcpy(output + AESGCM_MAC_SIZE, gcm_iv, AESGCM_IV_SIZE);

    int ciphertext_len=0, final_len=0;

    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_aes_128_gcm(),key, gcm_iv);

    EVP_EncryptUpdate(ctx, output + AESGCM_MAC_SIZE+ AESGCM_IV_SIZE, &ciphertext_len, plaintext, plaintext_len);
    EVP_EncryptFinal(ctx, output + AESGCM_MAC_SIZE + AESGCM_IV_SIZE + ciphertext_len, &final_len);
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AESGCM_MAC_SIZE, output);
    EVP_CIPHER_CTX_free(ctx);

    ciphertext_len = AESGCM_MAC_SIZE + AESGCM_IV_SIZE + ciphertext_len + final_len;
    memcpy(ciphertext,output,ciphertext_len);

    return ciphertext_len;

}

int dec_aes_gcm(unsigned char *ciphertext, int ciphertext_len,
                unsigned char *key,
                unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int plaintext_len=0, final_len=0;

    EVP_DecryptInit(ctx, EVP_aes_128_gcm(), key, gcm_iv);
    EVP_DecryptUpdate(ctx, plaintext, &plaintext_len,
                      ciphertext+AESGCM_MAC_SIZE+AESGCM_IV_SIZE,
                      ciphertext_len - AESGCM_MAC_SIZE - AESGCM_IV_SIZE);

    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AESGCM_MAC_SIZE, ciphertext);
    EVP_DecryptFinal(ctx, plaintext + plaintext_len, &final_len);
    EVP_CIPHER_CTX_free(ctx);
    plaintext_len = plaintext_len + final_len;

    return plaintext_len;
}
