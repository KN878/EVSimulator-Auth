//
// Created by nikita on 01.05.19.
//

#include "Car.h"

Car::Car(OBU obu) {
    this->obu = std::move(obu);
}

bool Car::registerInCSPA() {
    RSA::PublicKey publicKey = this->obu.getPublicKey();
    string cert;
    StringSink stringSink(cert);
    publicKey.Save(stringSink);

    CryptoPP::HexEncoder encoder;
    string ps, obuHash, readableCert;

    encoder.Attach( new CryptoPP::StringSink( ps ) );
    encoder.Put( (byte *)this->obu.getPseudonym().c_str(), this->obu.getPseudonym().length());
    encoder.MessageEnd();
    encoder.Attach( new CryptoPP::StringSink( obuHash ) );
    encoder.Put( (byte *)this->obu.getOBUHash().c_str(), this->obu.getOBUHash().length());
    encoder.MessageEnd();
    encoder.Attach( new CryptoPP::StringSink( readableCert ) );
    encoder.Put( (byte *)cert.c_str(), cert.length());
    encoder.MessageEnd();


    cout << "Registering with Pseudonym: " << ps << endl;
    cout << "with OBU hash: " << obuHash << endl;
    cout << "with OBU certificate: " << readableCert << endl;

    return this->cspa.registerCar(this->obu.getPseudonym().c_str(), this->obu.getOBUHash().c_str(), cert.c_str());
}

void Car::authenticate(ChargingStation cs) {
    string preHead, obuHash;
    CryptoPP::HexEncoder encoder;
    encoder.Attach( new CryptoPP::StringSink( preHead ) );
    encoder.Put( (byte *)this->obu.getPseudonymPreHead().c_str(), this->obu.getPseudonymPreHead().length());
    encoder.MessageEnd();
    encoder.Attach( new CryptoPP::StringSink( obuHash ) );
    encoder.Put( (byte *)this->obu.getOBUHash().c_str(), this->obu.getOBUHash().length());
    encoder.MessageEnd();

    cout << "Sending a request to CS with pseudonym pre-head: " << preHead << endl;
    cout << "With OBU hash: " << obuHash << endl;
    vector<byte> reply =
            cs.authenticate(this->obu.getPseudonymPreHead().c_str(), this->obu.getOBUHash().c_str(),
                            this->obu.getPublicKey());
    if (reply.size() != 1) {
        string readable;
        cout << "Car authenticated successfully" << endl;
        string encryptedKey(reply.begin(), reply.end());
        CryptoPP::HexEncoder encoder;
        encoder.Attach( new CryptoPP::StringSink( readable ) );
        encoder.Put( (byte*)encryptedKey.c_str(), encryptedKey.length());
        encoder.MessageEnd();
        cout << "Received encrypted session key: " << readable << endl;
        this->obu.setSessionKey(encryptedKey);
    } else {
        cout << "Car authentication rejected" << endl;
    }
}