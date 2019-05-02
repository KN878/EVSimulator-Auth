//
// Created by nikita on 01.05.19.
//

#include "Car.h"

Car::Car(OBU obu) {
    this->obu = std::move(obu);
}

void Car::registerInCSPA() {
    RSA::PublicKey publicKey = this->obu.getPublicKey();
    string cert;
    StringSink stringSink(cert);
    publicKey.Save(stringSink);

    this->cspa.registerCar(this->obu.getPseudonym().c_str(), this->obu.getOBUHash().c_str(), cert.c_str());
}

void Car::authenticate(ChargingStation cs) {
    vector<byte> reply =
            cs.authenticate(this->obu.getPseudonymPreHead().c_str(), this->obu.getOBUHash().c_str(),
                            this->obu.getPublicKey());
    if (reply.size() != 1) {
        this->obu.setSessionKey(string(reply.begin(), reply.end()));
    }
}