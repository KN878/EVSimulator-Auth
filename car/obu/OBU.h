//
// Created by nikita on 01.05.19.
//

#ifndef EVSIMULATOR_OBU_H
#define EVSIMULATOR_OBU_H

#include <string>
#include <cryptopp/rsa.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include "../../car/trm/TRM.h"

using namespace CryptoPP;
using namespace std;

class OBU {
private:
    RSA::PrivateKey privateKey;
    RSA::PublicKey publicKey;
    SecByteBlock sessionKey;
    TRM trm;
    string wallet;

    void generateKeyPair();

public:
    OBU();

    explicit OBU(TRM trm);

    RSA::PublicKey getPublicKey();

    RSAES_OAEP_SHA_Encryptor getEncryptor();

    void setSessionKey(const string encryptedKey);

    void setWallet(const string wallet);

    string getWallet();

    string getPseudonym();

    string getOBUHash();

    string getPseudonymPreHead();
};


#endif //EVSIMULATOR_OBU_H
