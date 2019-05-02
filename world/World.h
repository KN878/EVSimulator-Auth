//
// Created by nikita on 01.05.19.
//

#ifndef EVSIMULATOR_WORLD_H
#define EVSIMULATOR_WORLD_H

#include "../car/Car.h"
#include "../dmv/DMV.h"
#include <vector>

using namespace std;

class World {
private:
    vector<Car> cars;
    DMV &dmv = DMV::getInstance();
    CSPA &cspa = CSPA::getInstance();
    vector<ChargingStation> chargingStations;
public:
    void addNewCar();

    void addChargingStations(const int count);

    ChargingStation &getChargingStation(const int pos);

    Car getCar(const int pos);
};


#endif //EVSIMULATOR_WORLD_H
