#include <iostream>
#include <array>
#include "cryptopp/sha.h"
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <string>
#include <chrono>

using namespace CryptoPP;
using namespace std;

void generatePSK(array<byte[SHA256::DIGESTSIZE], 5> *hashes) {
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            SecByteBlock nonce(16);
            OS_GenerateRandomBlock(false, nonce, nonce.size());
            SHA256().CalculateDigest(hashes->at(i), nonce, nonce.size());
        } else {
            SHA256().CalculateDigest(hashes->at(i), hashes->at(i - 1), SHA256::DIGESTSIZE);
        }
    }
}


void generateASCreds(const char *id, const byte *pwXor, const byte *psk) {
    byte a[SHA256::DIGESTSIZE];
    byte b[SHA256::DIGESTSIZE];
    byte c[SHA256::DIGESTSIZE];
    byte k[SHA256::DIGESTSIZE];
    byte D[SHA256::DIGESTSIZE];

    SecByteBlock nonce(32);
    OS_GenerateRandomBlock(false, nonce, nonce.size());

    byte idXorNonce[strlen(id) + nonce.size()];
    memcpy(idXorNonce, id, strlen(id));
    memcpy(idXorNonce + strlen(id), nonce, nonce.size());
    //Generate a
    SHA256().CalculateDigest(a, idXorNonce, strlen(id) + nonce.size());
    //Generate b
    SHA256().CalculateDigest(b, a, sizeof(a));

    //Generate c
    for (int i = 0; i < SHA256::DIGESTSIZE; i++) {
        c[i] = pwXor[i] ^ b[i];
    }

    //Generate k
    for (int i = 0; i < SHA256::DIGESTSIZE; i++) {
        k[i] = psk[i] ^ a[i];
    }

    //Generate D
    unsigned long long now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    const char *timestamp = &to_string(now)[0];
    byte concatPskTimestamp[SHA256::DIGESTSIZE + strlen(timestamp)];
    memcpy(concatPskTimestamp, psk, SHA256::DIGESTSIZE);
    memcpy(concatPskTimestamp + SHA256::DIGESTSIZE, timestamp, strlen(timestamp));
    SHA256().CalculateDigest(D, concatPskTimestamp, SHA256::DIGESTSIZE + strlen(timestamp));
    for (int i = 0; i < SHA256::DIGESTSIZE; i++) {
        D[i] ^= id[i];
    }

}

void generateUserCredsInCar(const char *id, const char *pw, const byte *psk) {
    SecByteBlock u(32);
    OS_GenerateRandomBlock(false, u, u.size());

    string str(reinterpret_cast<const char *>(&u[0]), u.size());
    byte nonce[32];
    copy(str.begin(), str.end(), nonce);

    byte pwHash[SHA256::DIGESTSIZE];
    SHA256().CalculateDigest(pwHash, (byte *) pw, strlen(pw));

    byte pwXor[32];
    for (int i = 0; i < 32; i++) {
        pwXor[i] = pwHash[i] ^ nonce[i];
    }

    generateASCreds(id, pwXor, psk);
}

int main() {
    array<byte[SHA256::DIGESTSIZE], 5> hashes;

    generatePSK(&hashes);

    const char *id = "id";
    const char *pw = "pw";
    generateUserCredsInCar(id, pw, hashes.at(0));

    return 0;
}