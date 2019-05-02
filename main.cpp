#include <iostream>
#include "world/World.h"
using namespace std;

void registerCarsInCSPA(World world) {
    for (int i=0; i<50; i++) {
        cout << "Car with id " << i << " registering in CSPA..." << endl;
        if (world.getCar(i).registerInCSPA()) {
            cout << "Car with id " << i << " successfully registered" << endl << endl;
        } else {
            cout << "Car with id " << i << " registration rejected" << endl << endl;
        }
    }
}

void authenticateCars(World world, const int count, const int csPos) {
    for(int i=0; i< count; i++) {
        cout << endl << "Car with id " << i << " tries to authenticate with CS id" << csPos << endl;
        world.getCar(i).authenticate(world.getChargingStation(csPos));
    }
}

int main() {
    World world;
    world.addNewCarS(50);
    world.addChargingStations(3);
    registerCarsInCSPA(world);
    authenticateCars(world, 3, 0);
    return 0;
}
