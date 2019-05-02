//
// Created by nikita on 01.05.19.
// CSPA - Charging service providing authority (controls all charging stations)
//

#ifndef EVSIMULATOR_CSPA_H
#define EVSIMULATOR_CSPA_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>

using namespace std;
using namespace CryptoPP;

class CSPA;

class CSPADestroyer {
private:
    CSPA *instance;
public:
    ~CSPADestroyer();

    void initialize(CSPA *instance);

};

class CSPA {
private:
    static CSPA *instance;
    static CSPADestroyer destroyer;
    unordered_map<string, string> certMap; //map that connects Cert_OBU and X_OBU (replaces certification authority)
    // and used during cars' registration
    unordered_map<string, string> pseudonymsMap; //map that connects X_OBU and head of the pseudonym's hash

    vector<byte> generateSessionKey();

    vector<byte> encryptSessionKey(const byte *key, const RSA::PublicKey &publicKey);

protected:
    CSPA() {}

    ~CSPA() {}

    friend class CSPADestroyer;

public:
    static CSPA &getInstance();

    void registerCertificate(const char *cert); // replaces certification authority
    bool registerCar(const char *pseudonymHash, const char *hashOBU, const char *cert);

    vector<byte> authenticateCar(const char *preHead, const char *hashOBU, const RSA::PublicKey &publicKey);
};


#endif //EVSIMULATOR_CSPA_H
