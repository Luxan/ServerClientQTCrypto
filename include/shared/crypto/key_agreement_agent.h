#pragma once

#include <sstream>

#include <cryptopp/dh.h>
#include <cryptopp/dh2.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/oids.h>
#include <cryptopp/queue.h>
#include <cryptopp/asn.h>
#include <cryptopp/rsa.h>
#include <cryptopp/md5.h>

#define DEFINE_OID2(value, name)	inline CryptoPP::OID name() {return value;}

// a smart pointer for PK_Verifier so less cleanup is needed if something throws
typedef std::auto_ptr<CryptoPP::PK_Verifier> PK_VerifierPtr;

// Signature Algorithm OIDs commonly used in certs that have RSA keys
DEFINE_OID2(CryptoPP::ASN1::pkcs_1()+4, md5withRSAEncryption);
DEFINE_OID2(CryptoPP::ASN1::pkcs_1()+5, sha1withRSAEncryption);

#include "cipher.h"
#include "certificate.h"
#include "certificate_authority.h"

class KeyAgreamentAgent
{
private:
    Certificate * certificate;
    CertificateAuthority * ca;
    CryptoPP::ByteQueue caBQ, signedServerKey;
    CryptoPP::Integer p, g, q;
    CryptoPP::DH dh;
    CryptoPP::DH2 * dh2;
    CryptoPP::SecByteBlock staticPrivateKey, staticPublicKey;
    CryptoPP::SecByteBlock ephemeralPrivateKey, ephemeralPublicKey;
    CryptoPP::AutoSeededRandomPool rnd;

    void GetPublicKeyFromCert(CryptoPP::BufferedTransformation & certin, CryptoPP::BufferedTransformation & keyout)
    {
           CryptoPP::BERSequenceDecoder x509Cert(certin);
           CryptoPP::BERSequenceDecoder tbsCert(x509Cert);

           // ASN.1 from RFC 3280
           // TBSCertificate  ::=  SEQUENCE  {
           // version         [0]  EXPLICIT Version DEFAULT v1,

           // consume the context tag on the version
           CryptoPP::BERGeneralDecoder context(tbsCert,0xa0);
           CryptoPP::word32 ver;

           // only want a v3 cert
           CryptoPP::BERDecodeUnsigned<CryptoPP::word32>(context,ver,CryptoPP::INTEGER,2,2);

           // serialNumber         CertificateSerialNumber,
           CryptoPP::Integer serial;
           serial.BERDecode(tbsCert);

           // signature            AlgorithmIdentifier,
           CryptoPP::BERSequenceDecoder signature(tbsCert);
           signature.SkipAll();

           // issuer               Name,
           CryptoPP::BERSequenceDecoder issuerName(tbsCert);
           issuerName.SkipAll();

           // validity             Validity,
           CryptoPP::BERSequenceDecoder validity(tbsCert);
           validity.SkipAll();

           // subject              Name,
           CryptoPP::BERSequenceDecoder subjectName(tbsCert);
           subjectName.SkipAll();

           // subjectPublicKeyInfo SubjectPublicKeyInfo,
           CryptoPP::BERSequenceDecoder spki(tbsCert);
           CryptoPP::DERSequenceEncoder spkiEncoder(keyout);

           spki.CopyTo(spkiEncoder);
           spkiEncoder.MessageEnd();

           spki.SkipAll();
           tbsCert.SkipAll();
           x509Cert.SkipAll();
    }

    Key * authorize(Certificate * cert)
    {
        CryptoPP::ByteQueue servCertBQ, encodedServCertBQ, caKey;
        CryptoPP::SecByteBlock certSignature;

        servCertBQ.Put(cert->getBuffer()->getPointerToBuffer(), cert->getBuffer()->getLength());

        try
        {
            GetPublicKeyFromCert(caBQ, caKey);
        }
        catch(std::exception &)
        {
            throw("Failed to extract the public key from the CA certificate.");
            //return false;
        }
        //Public key read from serv CA certificate.

        CryptoPP::OID sigAlgOID;

        try
        {
               // first, extract the data that the signature covers
               CryptoPP::BERSequenceDecoder x509Cert(servCertBQ);
               CryptoPP::BERSequenceDecoder servCert(x509Cert);
               CryptoPP::DERSequenceEncoder encodedServCert(encodedServCertBQ);
               servCert.TransferAllTo(encodedServCert);
               encodedServCert.MessageEnd();
               // find the algorithm used to sign the data
               CryptoPP::BERSequenceDecoder sigAlg(x509Cert);
               sigAlgOID.BERDecode(sigAlg);
               sigAlg.SkipAll();
               // extract the actual signature
               uint32_t unused = 0;
               CryptoPP::BERDecodeBitString(x509Cert, certSignature, unused);
               //cout << "Signature decoded. " << unused << " bits unused in the encoding." << endl;
               x509Cert.SkipAll();
        }
        catch(std::exception &)
        {
               throw("Error decoding server's certificate for signature verification.");
               //return false;
        }
        PK_VerifierPtr verifier;
        if(sigAlgOID == md5withRSAEncryption())
        {
               verifier = PK_VerifierPtr(new CryptoPP::RSASS<CryptoPP::PKCS1v15,CryptoPP::MD5>::Verifier(caKey));
               //cout << "Signature algorithm is RSA with MD5 hash." << endl;
        }
        else if(sigAlgOID == sha1withRSAEncryption())
        {
               verifier = PK_VerifierPtr(new CryptoPP::RSASS<CryptoPP::PKCS1v15,CryptoPP::SHA1>::Verifier(caKey));
               //cout << "Signature algorithm is RSA with SHA1 hash." << endl;
        }
        else
        {
               throw("This program does not support the algorithm used for signing.");
               //return false;
        }

        if(certSignature.size() != verifier->SignatureLength())
        {
               throw("The signature size is does not match the algorithm used for signing.");
               //return false;
        }
        CryptoPP::SignatureVerificationFilter vf(*verifier);
        try
        {
               vf.Put(certSignature,certSignature.size());
               encodedServCertBQ.TransferAllTo(vf);
               vf.MessageEnd();
        }
        catch(std::exception &e)
        {
               throw(std::string("Caught an exception while verifying the signature:\t") + e.what());
               //return false;
        }
        if(vf.GetLastResult())
        {
            //cout << "The signature verified." << endl;
            CryptoPP::ByteQueue serverKey, servercert;
            servercert.Put(cert->getBuffer()->getPointerToBuffer(), cert->getBuffer()->getLength());

            try
            {
                    GetPublicKeyFromCert(servercert, serverKey);
            }
            catch(std::exception &)
            {
                    throw "Failed to extract the public key from the Server certificate.";
            }
            CryptoPP::RSA::PublicKey publicKey;
            publicKey.BERDecode(serverKey);
            uint8_t buffer[8192];
            CryptoPP::ArraySink arraySink(&buffer[0], 8192);
            publicKey.DEREncode(arraySink);
            return new Key(buffer, arraySink.TotalPutLength(), Key::EncryptionKeyType::RSAPrivate);
        }

        //Signature verification failed

        return nullptr;
    }

    void generatePGQ()
    {
        dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, 1024);

        p = dh.GetGroupParameters().GetModulus();
        q = (p-1)/2;
        g = dh.GetGroupParameters().GetGenerator();
    }

public:
    KeyAgreamentAgent(Certificate * cert):
        certificate(cert), p((long)0), g((long)0), q((long)0)
    {
        ca = nullptr;
        dh2 = nullptr;
    }

    KeyAgreamentAgent(CertificateAuthority * ca):
        ca(ca)
    {
        generatePGQ();
        caBQ.Put(ca->getBuffer()->getPointerToBuffer(), ca->getBuffer()->getLength());
        certificate = nullptr;
        dh2 = nullptr;
    }

    ~KeyAgreamentAgent()
    {
        if (certificate != nullptr)
            delete certificate;
        if (ca != nullptr)
            delete ca;
        if (dh2 != nullptr)
            delete dh2;
    }

    Key * verifyCertificate(Certificate * cert)
    {
        Key *key;
        if(ca == nullptr)
        {
            throw "CertificateAuthority is not present in KeyAgreamentAgent!";
        }
        if (cert == nullptr)
        {
            throw "certificate is Null";
        }
        if (key = authorize(cert))
        {
            certificate = cert;
            return key;
        }
        return nullptr;
    }

    Buffer * getP()
    {
        std::stringstream ss;
        ss << p;
        std::string s = ss.str();
        return new Buffer((uint8_t*)s.c_str(), s.size());
    }

    Buffer * getG()
    {
        std::stringstream ss;
        ss << g;
        std::string s = ss.str();
        return new Buffer((uint8_t*)s.c_str(), s.size());
    }

    Buffer * getQ()
    {
        std::stringstream ss;
        ss << q;
        std::string s = ss.str();
        return new Buffer((uint8_t*)s.c_str(), s.size());
    }

    bool verifyPGQ(Buffer * _p, Buffer * _g, Buffer * _q)
    {
        p = CryptoPP::Integer(_p->getPointerToBuffer(), _p->getLength());
        g = CryptoPP::Integer(_g->getPointerToBuffer(), _g->getLength());
        q = CryptoPP::Integer(_q->getPointerToBuffer(), _q->getLength());

        dh.AccessGroupParameters().Initialize(p, q, g);

        if(!dh.GetGroupParameters().ValidateGroup(rnd, 3))
            throw "Failed to validate prime and generator";

        CryptoPP::Integer v = ModularExponentiation(g, q, p);
        if(v != CryptoPP::Integer::One())
            throw "Failed to verify order of the subgroup";

        return true;
    }

    void prepareStaticAndEphemeralKeys()
    {
        staticPrivateKey = CryptoPP::SecByteBlock(dh2->StaticPrivateKeyLength());
        staticPublicKey = CryptoPP::SecByteBlock(dh2->StaticPublicKeyLength());

        ephemeralPrivateKey = CryptoPP::SecByteBlock(dh2->EphemeralPrivateKeyLength());
        ephemeralPublicKey = CryptoPP::SecByteBlock(dh2->EphemeralPublicKeyLength());

        dh2->GenerateStaticKeyPair(rnd, staticPrivateKey, staticPublicKey);
        dh2->GenerateEphemeralKeyPair(rnd, ephemeralPrivateKey, ephemeralPublicKey);
    }

    uint32_t getAgreedValueLength()
    {
        return dh2->AgreedValueLength();
    }

    Key * getStaticPublicKey()
    {
        return new Key((uint8_t*)staticPublicKey.BytePtr(), staticPublicKey.SizeInBytes());
    }

    Key * getEphemeralPublicKey()
    {
        return new Key((uint8_t*)ephemeralPublicKey.BytePtr(), ephemeralPublicKey.SizeInBytes());
    }

    Key * retrieveSecretKey(uint32_t agreedValueLength, Buffer * spubB, Buffer * epubB)
    {
        if(dh2->AgreedValueLength() != agreedValueLength)
            throw "Shared secret size mismatch";

        CryptoPP::SecByteBlock remoteStaticPublicKey(spubB->getPointerToBuffer(), spubB->getLength());
        CryptoPP::SecByteBlock remoteEphemeralPublicKey(epubB->getPointerToBuffer(), epubB->getLength());

        CryptoPP::SecByteBlock sharedKey(agreedValueLength);

        if(!dh2->Agree(sharedKey, staticPrivateKey, ephemeralPrivateKey, remoteStaticPublicKey, remoteEphemeralPublicKey))
            throw "Failed to reach shared secret";

        return new Key((uint8_t*)sharedKey.BytePtr(), agreedValueLength);
    }
};
