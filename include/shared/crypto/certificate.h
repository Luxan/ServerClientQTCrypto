#pragma once

#include "../buffer.h"
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

class Certificate
{
    Buffer * certificate;
public:
    Certificate(Buffer * certificate):
        certificate(certificate)
    {}

    Buffer * getBuffer()
    {
        return certificate;
    }

    ~Certificate()
    {
        delete certificate;
    }
};

class CertificateAuthority
{
    Buffer * ca;
    Certificate * certificate;
    CryptoPP::ByteQueue caBQ, signedServerKey;
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
public:
    CertificateAuthority(Buffer * ca):
        ca(ca)
    {
        certificate = nullptr;
        caBQ.Put(this->getBuffer()->getPointerToBuffer(), this->getBuffer()->getLength());
    }

    ~CertificateAuthority()
    {
        if (certificate != nullptr)
            delete certificate;

        if (ca != nullptr)
            delete ca;
    }

    Buffer * getBuffer()
    {
        return ca;
    }

    bool authorize(Certificate * cert)
    {
        CryptoPP::ByteQueue servCertBQ, encodedServCertBQ, serverKey;
        CryptoPP::SecByteBlock certSignature;

        servCertBQ.Put(cert->getBuffer()->getPointerToBuffer(), cert->getBuffer()->getLength());

        try
        {
            GetPublicKeyFromCert(caBQ,serverKey);
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
               verifier = PK_VerifierPtr(new CryptoPP::RSASS<CryptoPP::PKCS1v15,CryptoPP::MD5>::Verifier(serverKey));
               //cout << "Signature algorithm is RSA with MD5 hash." << endl;
        }
        else if(sigAlgOID == sha1withRSAEncryption())
        {
               verifier = PK_VerifierPtr(new CryptoPP::RSASS<CryptoPP::PKCS1v15,CryptoPP::SHA1>::Verifier(serverKey));
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
            certificate = cert;
            signedServerKey = serverKey;
            return true;
        }

        //Signature verification failed

        return false;
    }
};
