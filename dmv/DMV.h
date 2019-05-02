//
// Created by nikita on 01.05.19.
//

#ifndef EVSIMULATOR_DMV_H
#define EVSIMULATOR_DMV_H

#include <vector>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include "../chargingStation/CSPA.h"
#include "../car/Car.h"

using namespace std;

class DMV;

class DMVDestroyer {
private:
    DMV *instance;
public:
    ~DMVDestroyer();

    void initialize(DMV *instance);
};

class DMV {
private:
    static const int DIM = 5;
    static DMV *instance;
    static DMVDestroyer destroyer;
    CSPA &cspa = CSPA::getInstance();
    RSA::PrivateKey privateKey;
    RSA::PublicKey publicKey;


    SecByteBlock generateSecretKey();

    vector<string> generatePSHashChain(const byte *ps, const int psLength);

    SecByteBlock generatePseudonym(
            const int secCounter,
            const int incFactor,
            const SecByteBlock &psKey,
            const SecByteBlock &carsKey,
            const int psCounter
    );

    string encryptAES(AutoSeededRandomPool *rnd, const SecByteBlock &key, const string &plain);

    string xorStrings(const string &str1, string str2);

protected:
    DMV() {}

    ~DMV() {}

    friend class DMVDestroyer;

public:
    static DMV &getInstance();

    Car createCar();
};

#endif //EVSIMULATOR_DMV_H
