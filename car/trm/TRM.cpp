//
// Created by nikita on 01.05.19.
//

#include "TRM.h"

void TRM::setSecretCounter(int secretCounter) {
    this->secretCounter = secretCounter;
}

void TRM::setIncrementingFactor(int incrementingFactor) {
    this->incrementingFactor = incrementingFactor;
}

void TRM::setObuHash(const byte *hash) {
    this->obuHash = string(reinterpret_cast<const char *>(hash), SHA256::DIGESTSIZE);
}

void TRM::setPseudonymsKey(SecByteBlock key) {
    this->pseudonymsKey = key;
}

void TRM::setSecretKey(SecByteBlock key) {
    this->secretKey = key;
}

void TRM::setPseudonyms(string pseudonymsHashes[DIM][DIM]) {
    std::copy(&pseudonymsHashes[0][0], &pseudonymsHashes[0][0] + DIM * DIM, &this->pseudonymsHashes[0][0]);
}

string TRM::getPseudonymHead() {
    return this->pseudonymsHashes[currentPseudonym][DIM - 1];
}

string TRM::getPseudonymPreHead() {
    return this->pseudonymsHashes[currentPseudonym][DIM - 2];
}

string TRM::getObuHash() {
    return this->obuHash;
}
