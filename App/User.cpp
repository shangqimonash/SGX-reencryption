/*
 * User.cpp
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#include "User.h"

User::User(char* user_name) {
	name = string(user_name);
	RAND_bytes(key, ENC_KEY_SIZE);
}

string User::get_name() {
	return name;
}

unsigned char* User::get_key() {
	return key;
}

void User::set_peer(char* peer_name) {
	this->peer_name = string(peer_name);
}

void User::send_to_peer(sgx_enclave_id_t eid, char* message) {
	if(peer_name.size() != 0) {
		printf("Send message: %s\n", message);
		/* generate the ciphertext from message */
		unsigned char *ciphertext;
		int cipher_len;
		ciphertext = (unsigned char*) malloc(strlen(message) + 1 + AESGCM_MAC_SIZE + AESGCM_IV_SIZE);
		cipher_len = enc_aes_gcm((unsigned char*) message, strlen(message) + 1, key, ciphertext);

		/* ecall to send the ciphertext to the enclave */
		ecall_send_message(eid, ciphertext, cipher_len,
				(char*) name.c_str(), name.size(),
				(char*) peer_name.c_str(), peer_name.size());
	}
}
