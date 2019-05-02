//
// Created by nikita on 01.05.19.
//

#include "DMV.h"

DMV *DMV::instance = nullptr;
DMVDestroyer DMV::destroyer;

DMVDestroyer::~DMVDestroyer() {
    delete instance;
}

void DMVDestroyer::initialize(DMV *dmv) {
    instance = dmv;
}

DMV &DMV::getInstance() {
    if (!instance) {
        instance = new DMV();
        destroyer.initialize(instance);

        AutoSeededRandomPool rnd;
        instance->privateKey.GenerateRandomWithKeySize(rnd, 3072);
        instance->publicKey = RSA::PublicKey(instance->privateKey);
    }
    return *instance;
}

Car DMV::createCar() {
    //creating secret params for car's TRM
    AutoSeededRandomPool rnd;
    Integer secretCounter(rnd, 1, 10000);
    Integer incrementingFactor(rnd, 1, 10000);

    SecByteBlock secretKey = generateSecretKey();
    SecByteBlock pseudonymsKey = generateSecretKey();

    string psHashes[DIM][DIM];
    vector<SecByteBlock> pseudonyms;
    for (int i = 0; i < DIM; i++) {
        pseudonyms.emplace_back(generatePseudonym(
                static_cast<int>(secretCounter.ConvertToLong()),
                static_cast<int>(incrementingFactor.ConvertToLong()),
                pseudonymsKey,
                secretKey,
                i + 1));
        vector<string> psHashChain =
                generatePSHashChain(pseudonyms[i], pseudonyms[i].size());
        for (int j = 0; j < DIM; j++) {
            psHashes[i][j] = psHashChain[j];
        }
    }

    string concatedPS;
    for (int i = 0; i < DIM; i++) {
        concatedPS.append(
                reinterpret_cast<const char *>(pseudonyms[i].BytePtr()),
                pseudonyms[i].size());
    }

    byte hashOBU[SHA256::DIGESTSIZE];
    SHA256().CalculateDigest(hashOBU, (byte *) concatedPS.c_str(), concatedPS.length());

    TRM trm;
    trm.setSecretCounter(static_cast<int>(secretCounter.ConvertToLong()));
    trm.setIncrementingFactor(static_cast<int>(incrementingFactor.ConvertToLong()));
    trm.setSecretKey(secretKey);
    trm.setPseudonymsKey(pseudonymsKey);
    trm.setObuHash(hashOBU);
    trm.setPseudonyms(psHashes);

    OBU obu(trm);
    RSA::PublicKey obuPublicKey = obu.getPublicKey();
    string cert;
    StringSink stringSink(cert);
    obuPublicKey.Save(stringSink);
    cspa.registerCertificate(cert.c_str());
    return Car(obu);
}

SecByteBlock DMV::generateSecretKey() {
    AutoSeededRandomPool rnd;
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    rnd.GenerateBlock(key, AES::DEFAULT_KEYLENGTH);
    return key;
}

SecByteBlock DMV::generatePseudonym(
        const int secCounter,
        const int incFactor,
        const SecByteBlock &psKey,
        const SecByteBlock &carsKey,
        const int psCounter) {
    AutoSeededRandomPool rnd;

    string a = to_string(secCounter + psCounter * incFactor);
    string ID = to_string(Integer(rnd, 1, 1000000).ConvertToLong());

    string cipheredA = encryptAES(&rnd, psKey, a);
    string aXorID = xorStrings(a, ID);
    string cipheredXor = encryptAES(&rnd, carsKey, aXorID);
    string final = cipheredA + cipheredXor + to_string(psCounter);

    RSASSA_PKCS1v15_SHA_Signer privKey(instance->privateKey);
    SecByteBlock signature(privKey.SignatureLength());
    privKey.SignMessage(
            rnd,
            (const byte *) final.data(),
            final.length(),
            signature
    );
    return signature;
}

vector<string> DMV::generatePSHashChain(const byte *ps, const int psLength) {
    unsigned char tempSHA[SHA256::DIGESTSIZE];
    vector<string> psHashChain;
    for (int i = 0; i < DIM; i++) {
        if (i == 0) {
            SHA256().CalculateDigest(tempSHA, ps, psLength);
        } else {
            string input = psHashChain[i - 1];
            SHA256().CalculateDigest(tempSHA, (byte *) &input[0], SHA256::DIGESTSIZE);
        }
        psHashChain.emplace_back(reinterpret_cast<const char *>(&tempSHA[0]), sizeof(tempSHA));
    }
    return psHashChain;
}

string DMV::encryptAES(AutoSeededRandomPool *rnd, const SecByteBlock &key, const string &plain) {
    byte iv[AES::BLOCKSIZE];
    rnd->GenerateBlock(iv, AES::BLOCKSIZE);

    string cipher;
    AES::Encryption aesEncryption(key, AES::DEFAULT_KEYLENGTH);
    CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
    StreamTransformationFilter stfEncryptor(cbcEncryption, new StringSink(cipher));
    stfEncryptor.Put(reinterpret_cast<const unsigned char *>(plain.c_str()), plain.length());
    stfEncryptor.MessageEnd();
    return cipher;
}

string DMV::xorStrings(const string &str1, string str2) {
    int maxStr = max(str1.length(), str2.length());
    char xored[maxStr + 1];
    for (int i = 0; i < maxStr; i++) {
        if (i >= str1.length()) {
            xored[i] = '0' ^ str2[i];
        } else if (i >= str2.length()) {
            xored[i] = '0' ^ str1[i];
        } else {
            xored[i] = str1[i] ^ str2[i];
        }
    }
    xored[maxStr] = '\0';
    return xored;
}
