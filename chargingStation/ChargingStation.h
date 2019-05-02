//
// Created by nikita on 01.05.19.
//

#ifndef EVSIMULATOR_CHARGINGSTATION_H
#define EVSIMULATOR_CHARGINGSTATION_H


#include "CSPA.h"

class ChargingStation {
private:
    unordered_map<string, string> keysMap;
    CSPA &cspa = CSPA::getInstance();
public:
    vector<byte> authenticate(const char *preHead, const char *hashOBU, const RSA::PublicKey &publicKey);
};


#endif //EVSIMULATOR_CHARGINGSTATION_H
