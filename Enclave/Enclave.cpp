/*
 * Enclave.cpp
 *
 *  Created on: 3 Sep 2019
 *      Author: shangqi
 */
#include <string>
#include <unordered_map>

#include "sgx_trts.h"
#include "Enclave_t.h"

#include "EnclaveUtils.h"

using namespace std;

unordered_map<string, string> key_map;
unordered_map<string, string> buffer;

void ecall_register_user(char *name, size_t name_len, unsigned char *K, size_t K_len) {
	string user_name = string(name, name_len);
	string user_key = string((char*) K, K_len);
	key_map[user_name] = user_key;
}

void ecall_send_message(unsigned char* ciphertext, size_t cipher_len, char* user_name, size_t uname_len, char* peer_name, size_t pname_len) {
	/* retrieve the user key */
	string name = string(user_name, uname_len);
	string user_key = key_map[name];

	/* decrypt the ciphertext within the enclave */
	rand_t plaintext;
	prf_Dec(user_key.c_str(), ciphertext, cipher_len, &plaintext);
	//ocall_print_string((char*) plaintext.content);

	/* retrieve the peer key */
	string peer = string(peer_name, pname_len);
	string peer_key = key_map[peer];

	/* generate the ciphertext for the peer */
	rand_t peer_ciphertext;
	prf_Enc(peer_key.c_str(), plaintext.content, plaintext.content_length, &peer_ciphertext);
	buffer[peer] = string((char*) peer_ciphertext.content, peer_ciphertext.content_length);
}
