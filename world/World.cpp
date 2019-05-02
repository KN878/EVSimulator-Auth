//
// Created by nikita on 01.05.19.
//

#include "World.h"

void World::addNewCar() {
    this->cars.push_back(this->dmv.createCar());
}

void World::addChargingStations(const int count) {
    for (int i = 0; i < count; i++) {
        this->chargingStations.emplace_back();
    }
}

ChargingStation &World::getChargingStation(const int pos) {
    return this->chargingStations[pos];
}

Car World::getCar(const int pos) {
    return this->cars[pos];
}