//
// Created by nikita on 01.05.19.
//

#include "ChargingStation.h"

vector<byte> ChargingStation::authenticate(const char *preHead, const char *hashOBU, const RSA::PublicKey &publicKey) {
    vector<byte> reply = cspa.authenticateCar(preHead, hashOBU, publicKey);
    if (reply[0] == 0x01) {
        vector<byte> sessionKey(reply.begin() + 1, reply.begin() + AES::DEFAULT_KEYLENGTH + 1);
        this->keysMap[hashOBU] = string(sessionKey.begin(), sessionKey.end());
        vector<byte> encryptedKey(reply.begin() + AES::DEFAULT_KEYLENGTH + 1, reply.end());
        return encryptedKey;
    } else {
        return reply;
    }
}