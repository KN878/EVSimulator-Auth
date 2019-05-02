//
// Created by nikita on 01.05.19.
//

#ifndef EVSIMULATOR_TRM_H
#define EVSIMULATOR_TRM_H

#include <string>
#include <cryptopp/secblock.h>
#include <cryptopp/sha.h>
#include <cryptopp/aes.h>

using namespace std;
using namespace CryptoPP;

class TRM {
private:
    static const unsigned int DIM = 5;

    int secretCounter;
    int incrementingFactor;
    int currentPseudonym = 0;
    SecByteBlock secretKey;
    SecByteBlock pseudonymsKey;
    string obuHash;
    string pseudonymsHashes[DIM][DIM];
public:
    void setSecretCounter(int secretCounter);

    void setIncrementingFactor(int incrementingFactor);

    void setSecretKey(SecByteBlock key);

    void setPseudonymsKey(SecByteBlock key);

    void setObuHash(const byte *hash);

    string getObuHash();

    void setPseudonyms(string pseudonymsHashes[DIM][DIM]);

    string getPseudonymHead();

    string getPseudonymPreHead();

};


#endif //EVSIMULATOR_TRM_H
