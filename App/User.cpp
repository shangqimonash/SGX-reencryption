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
		printf("%s sends: %s\n", name.c_str(), message);
		/* generate the ciphertext from message */
		unsigned char* ciphertext = (unsigned char*) malloc(strlen(message) + 1 + AESGCM_MAC_SIZE + AESGCM_IV_SIZE);
		int cipher_len;
		cipher_len = enc_aes_gcm((unsigned char*) message, strlen(message) + 1, key, ciphertext);

		/* ecall to send the ciphertext to the enclave */
		ecall_reencrypt(eid, ciphertext, cipher_len,
				(char*) name.c_str(), name.size(),
				(char*) peer_name.c_str(), peer_name.size());
	}
}

void User::receive_message(rand_t* message) {
	/*decrypt the message with its own key */
	unsigned char* plaintext = (unsigned char*) malloc(message->content_length);
	int msg_len = dec_aes_gcm(message->content, message->content_length, key, plaintext);
	printf("%s receives: %s\n", name.c_str(), plaintext);
}
