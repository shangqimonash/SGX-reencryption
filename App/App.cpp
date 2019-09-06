/*
 * App.cpp
 * In this program, Alice wants to send some encrypted secret message to Bob.
 * However, Alice does not want to reveal her secret key to Bob to decrypt
 * the message. This program implements a re-encryption scheme via Intel SGX.
 * It provides an enclave keeping both Alice's and Bob's secret key. The
 * enclave is able to recover the encrypted secret message from Alice and
 * re-encrypt it with Bob's key. As a result, Bob can get the secret message
 * without knowing Alice's key, while the designated proxy cannot learn the
 * actually message as it is protected by SGX.
 *  Created on: 3 Sep 2019
 *      Author: shangqi
 */

#include <iostream>
#include <unordered_map>

#include "User.h"

using namespace std;

#define ENCLAVE_FILE "Enclave.signed.so"

unordered_map<string, User*> user_list;

void ocall_send_to_peer(char* peer_name, size_t pname_len, void* msg, size_t msg_len) {
	string peer = string(peer_name, pname_len);
	user_list[peer]->receive_message((rand_t*) msg);
}

int main() {
	/* Setup enclave */
	sgx_enclave_id_t eid;
	sgx_status_t ret;
	sgx_launch_token_t token = { 0 };
	int token_updated = 0;

	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &token_updated, &eid, NULL);
	if (ret != SGX_SUCCESS)
	{
		cout <<"sgx_create_enclave failed: 0x" << std::hex << ret << endl;
		return 1;
	}

	/* Initialise two-party */
	User *Alice = new User("Alice");
	ecall_register_user(eid, (char *) Alice->get_name().c_str(), Alice->get_name().size(), Alice->get_key(), ENC_KEY_SIZE);

	User *Bob = new User("Bob");
	ecall_register_user(eid, (char *) Bob->get_name().c_str(), Bob->get_name().size(), Bob->get_key(), ENC_KEY_SIZE);

	/* Register user and peer */
	user_list[Alice->get_name()] = Alice;
	user_list[Bob->get_name()] = Bob;

	Alice->set_peer("Bob");

	/* Send a test message */
	Alice->send_to_peer(eid, "This is a test message");
}

