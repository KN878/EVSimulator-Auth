#include <iostream>
#include "world/World.h"
using namespace std;

int main() {
    World world;
    world.addNewCar();
    world.addChargingStations(2);
    Car car = world.getCar(0);
    car.authenticate(world.getChargingStation(0));
    return 0;
}