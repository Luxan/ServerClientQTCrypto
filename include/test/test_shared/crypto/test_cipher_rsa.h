#pragma once

#include <QtTest/QtTest>
#include <QTest>

#include "../../../include/shared/crypto/cipher_rsa.h"
#include "../../../include/test/test_base.h"

class Test_RSACipher : public TestBase
{
    Q_OBJECT
    CLASSTEST(Test_RSACipher, TestBase)
private:
    RSACipher * cipher1, *cipher2, *cipher3, *cipher4, *cipher5;
    Key * pubKey, *privKey;
    PackageBuffer * input;
    PackageBuffer * hugeInput;
    PackageBuffer * output1, *output2, *output3, *output4, *output5;
    PackageBuffer * hugeoutput1, *hugeoutput2, *hugeoutput3, *hugeoutput4, *hugeoutput5;
    PackageBuffer * decodedoutput1, *decodedoutput2, *decodedoutput3, *decodedoutput4, *decodedoutput5;
    PackageBuffer * decodedhugeoutput1, *decodedhugeoutput2, *decodedhugeoutput3, *decodedhugeoutput4, *decodedhugeoutput5;

    bool enecodeHelper(Cipher* cipher, PackageBuffer * output, PackageBuffer * hugeoutput)
    {
        try
        {
            cipher->encrypt(input, output);
            cipher->encrypt(hugeInput, hugeoutput);
        }
        catch(const char * error)
        {
            return false;
        }
        return true;
    }

    bool decodeHelper(Cipher* cipher, PackageBuffer * decodedoutput, PackageBuffer * decodedhugeoutput)
    {
        try
        {
            cipher->decrypt(output1, decodedoutput);
            cipher->decrypt(hugeoutput1, decodedhugeoutput);
        }
        catch(const char * error)
        {
            return false;
        }
        return true;
    }

private slots:
    virtual void setup()
    {
        uint8_t tab[] = {0,1,2,3,4,5,6,7,8,9};
        input = new PackageBuffer(tab, sizeof(tab));
        uint8_t tab2[10000];
        for(int i = 0; i < sizeof(tab2); i++)
        {
            tab2[i] = i;
        }
        hugeInput = new PackageBuffer(tab2, sizeof(tab2));
        output1 = new PackageBuffer(nullptr, 0);
        output2 = new PackageBuffer(nullptr, 0);
        output3 = new PackageBuffer(nullptr, 0);
        output4 = new PackageBuffer(nullptr, 0);
        output5 = new PackageBuffer(nullptr, 0);

        hugeoutput1 = new PackageBuffer(nullptr, 0);
        hugeoutput2 = new PackageBuffer(nullptr, 0);
        hugeoutput3 = new PackageBuffer(nullptr, 0);
        hugeoutput4 = new PackageBuffer(nullptr, 0);
        hugeoutput5 = new PackageBuffer(nullptr, 0);

        decodedoutput1 = new PackageBuffer(nullptr, 0);
        decodedoutput2 = new PackageBuffer(nullptr, 0);
        decodedoutput3 = new PackageBuffer(nullptr, 0);
        decodedoutput4 = new PackageBuffer(nullptr, 0);
        decodedoutput5 = new PackageBuffer(nullptr, 0);

        decodedhugeoutput1 = new PackageBuffer(nullptr, 0);
        decodedhugeoutput2 = new PackageBuffer(nullptr, 0);
        decodedhugeoutput3 = new PackageBuffer(nullptr, 0);
        decodedhugeoutput4 = new PackageBuffer(nullptr, 0);
        decodedhugeoutput5 = new PackageBuffer(nullptr, 0);

        ///////////////////////////////////////
        // Generate Parameters
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::InvertibleRSAFunction params;
        params.GenerateRandomWithKeySize(rng, 4096);

        ///////////////////////////////////////
        // Create Keys
        CryptoPP::RSA::PrivateKey privateKey(params);
        CryptoPP::RSA::PublicKey publicKey(params);

        uint8_t buffer[8192];

        CryptoPP::ArraySink arraySink(&buffer[0], 8192);
        privateKey.DEREncode(arraySink);
        privKey = new Key(buffer, arraySink.TotalPutLength(), Key::EncryptionKeyType::RSAPrivate);
        CryptoPP::ArraySink arraySink2(&buffer[0], 8192);
        publicKey.DEREncode(arraySink2);
        pubKey = new Key(buffer, arraySink2.TotalPutLength(), Key::EncryptionKeyType::RSAPublic);
    }

    virtual void constructor()
    {
        cipher1 = new RSACipher();
        cipher2 = new RSACipher(nullptr, nullptr);
        cipher3 = new RSACipher(pubKey, nullptr);
        cipher4 = new RSACipher(nullptr, privKey);
        cipher5 = new RSACipher(pubKey, privKey);
    }
    void getPublicKey()
    {
        if (cipher1->getPublicKey() == nullptr)
        {
            Q_ASSERT(nullptr);
        }
        QCOMPARE(cipher2->getPublicKey(), (Key*)0);
        QVERIFY(*cipher3->getPublicKey() == *pubKey);
        QCOMPARE(cipher4->getPublicKey(), (Key*)0);
        QVERIFY(*cipher5->getPublicKey() == *pubKey);
    }

    void encrypt()
    {
        QVERIFY(enecodeHelper(cipher1, input, output1));
        QVERIFY(!enecodeHelper(cipher2, input, output2));
        QVERIFY(enecodeHelper(cipher3, input, output3));
        QVERIFY(!enecodeHelper(cipher4, input, output4));
        QVERIFY(enecodeHelper(cipher5, input, output5));

        QVERIFY(enecodeHelper(cipher1, hugeInput, hugeoutput1));
        QVERIFY(!enecodeHelper(cipher2, hugeInput, hugeoutput2));
        QVERIFY(enecodeHelper(cipher3, hugeInput, hugeoutput3));
        QVERIFY(!enecodeHelper(cipher4, hugeInput, hugeoutput4));
        QVERIFY(enecodeHelper(cipher5, hugeInput, hugeoutput5));
    }

    void decrypt()
    {
        QVERIFY(decodeHelper(cipher1, decodedoutput1, decodedhugeoutput1));
        QVERIFY(*input == *decodedoutput1);
        QVERIFY(*hugeInput == *decodedhugeoutput1);
        QVERIFY(!decodeHelper(cipher2, decodedoutput2, decodedhugeoutput2));
        QVERIFY(!decodeHelper(cipher3, decodedoutput3, decodedhugeoutput3));
        QVERIFY(decodeHelper(cipher4, decodedoutput4, decodedhugeoutput4));
        QVERIFY(decodeHelper(cipher5, decodedoutput5, decodedhugeoutput5));
        QVERIFY(*input == *decodedoutput5);
        QVERIFY(*hugeInput == *decodedhugeoutput5);
    }

    virtual void tearDown()
    {
        delete cipher1;
        delete cipher2;
        delete cipher3;
        delete cipher4;
        delete cipher5;
        delete pubKey;
        delete privKey;
        delete input;
        delete hugeInput;
        delete output1;
        delete output2;
        delete output3;
        delete output4;
        delete output5;
        delete hugeoutput1;
        delete hugeoutput2;
        delete hugeoutput3;
        delete hugeoutput4;
        delete hugeoutput5;
        delete decodedoutput1;
        delete decodedoutput2;
        delete decodedoutput3;
        delete decodedoutput4;
        delete decodedoutput5;
        delete decodedhugeoutput1;
        delete decodedhugeoutput2;
        delete decodedhugeoutput3;
        delete decodedhugeoutput4;
        delete decodedhugeoutput5;
    }
};
