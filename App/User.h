/*
 * User.h
 *
 *  Created on: 4 Sep 2019
 *      Author: shangqi
 */

#ifndef APP_USER_H_
#define APP_USER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "openssl/rand.h"
#include "sgx_urts.h"
#include "Enclave_u.h"

#include "Utils.h"

using namespace std;

class User {
public:
	User(char* user_name);
	string get_name();
	unsigned char* get_key();
	void set_peer(char* peer_name);
	void send_to_peer(sgx_enclave_id_t eid, char* message);
	void receive_message(rand_t* message);
private:
	string name;
	unsigned char key[ENC_KEY_SIZE];
	string peer_name;
};

#endif /* APP_USER_H_ */
