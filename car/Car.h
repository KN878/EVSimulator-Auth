//
// Created by nikita on 01.05.19.
//

#ifndef EVSIMULATOR_CAR_H
#define EVSIMULATOR_CAR_H

#include <cryptopp/hex.h>
#include "../car/obu/OBU.h"
#include "../chargingStation/CSPA.h"
#include "../chargingStation/ChargingStation.h"

class Car {
private:
    OBU obu;
    CSPA &cspa = CSPA::getInstance();
public:
    Car(OBU obu);

    bool registerInCSPA();

    void authenticate(ChargingStation cs);
};


#endif //EVSIMULATOR_CAR_H
