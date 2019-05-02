#include <utility>

//
// Created by nikita on 01.05.19.
//

#include "OBU.h"

OBU::OBU() = default;

OBU::OBU(TRM trm) {
    this->trm = std::move(trm);
    generateKeyPair();
}

void OBU::generateKeyPair() {
    AutoSeededRandomPool rng;
    InvertibleRSAFunction params;
    params.GenerateRandomWithKeySize(rng, 3072);
    RSA::PrivateKey privateKey(params);
    RSA::PublicKey publicKey(params);
    privateKey.ThrowIfInvalid(rng, 3);
    publicKey.ThrowIfInvalid(rng, 3);

    this->privateKey = privateKey;
    this->publicKey = publicKey;
}

void OBU::setWallet(const string wallet) {
    this->wallet = wallet;
}

string OBU::getWallet() {
    return this->wallet;
}

RSA::PublicKey OBU::getPublicKey() {
    return this->publicKey;
}

void OBU::setSessionKey(const string encryptedKey) {
    AutoSeededRandomPool rnd;
    string decrypted;
    RSAES_OAEP_SHA_Decryptor decryptor(this->privateKey);
    StringSource(encryptedKey, true, new PK_DecryptorFilter(rnd, decryptor, new StringSink(decrypted)));
    this->sessionKey = SecByteBlock(reinterpret_cast<const byte *>(&decrypted[0]), decrypted.size());
}

string OBU::getPseudonym() {
    return this->trm.getPseudonymHead();
}

string OBU::getOBUHash() {
    return this->trm.getObuHash();
}

string OBU::getPseudonymPreHead() {
    return this->trm.getPseudonymPreHead();
}

RSAES_OAEP_SHA_Encryptor OBU::getEncryptor() {
    RSAES_OAEP_SHA_Decryptor decryptor(this->privateKey);
    RSAES_OAEP_SHA_Encryptor encryptor(decryptor);
    return encryptor;
}