//
// Created by nikita on 01.05.19.
//

#include "CSPA.h"

CSPA *CSPA::instance = nullptr;
CSPADestroyer CSPA::destroyer;

CSPADestroyer::~CSPADestroyer() {
    delete instance;
}

void CSPADestroyer::initialize(CSPA *cspa) {
    instance = cspa;
}

CSPA &CSPA::getInstance() {
    if (!instance) {
        instance = new CSPA();
        destroyer.initialize(instance);
    }
    return *instance;
}

bool CSPA::registerCar(const char *pseudonymHash, const char *hashOBU, const char *cert) {
    if (this->certMap.find(cert) == this->certMap.end()) {
        return false;
    } else {
        this->pseudonymsMap[hashOBU] = pseudonymHash;
        return true;
    }
}

//First byte is the authentication state: 0x00 - not authenticated, 0x01 - authenticated
vector<byte> CSPA::authenticateCar(const char *preHead, const char *hashOBU, const RSA::PublicKey &publicKey) {
    byte calculatedHead[SHA256::DIGESTSIZE];
    SHA256().CalculateDigest(calculatedHead, (byte *) preHead, strlen(preHead));

    if (memcmp(calculatedHead, this->pseudonymsMap[hashOBU].c_str(), SHA256::DIGESTSIZE) == 0) {
        vector<byte> reply{0x01};
        vector<byte> key = generateSessionKey();
        vector<byte> encryptedKey = encryptSessionKey(key.data(), publicKey);


        reply.insert(
                reply.end(),
                make_move_iterator(key.begin()),
                make_move_iterator(key.end())
        );
        reply.insert(
                reply.end(),
                make_move_iterator(encryptedKey.begin()),
                make_move_iterator(encryptedKey.end())
        );
        return reply;
    } else {
        return vector<byte>{0x00};
    }
}

void CSPA::registerCertificate(const char *cert) {
    this->certMap[cert] = "";
}

vector<byte> CSPA::generateSessionKey() {
    AutoSeededRandomPool rnd;

    byte key[AES::DEFAULT_KEYLENGTH];
    rnd.GenerateBlock(key, sizeof(key));

    return vector<byte>(key, key + sizeof(key));
}

vector<byte> CSPA::encryptSessionKey(const byte *key, const RSA::PublicKey &publicKey) {
    RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
    AutoSeededRandomPool rnd;
    string cipher;
    StringSource((char *) key, true, new PK_EncryptorFilter(rnd, encryptor, new StringSink(cipher)));
    cout << "Encrypted session key: " << cipher << endl;

    return vector<byte>(&cipher[0], &cipher[0] + cipher.length());
}