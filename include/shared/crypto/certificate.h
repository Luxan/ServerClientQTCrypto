#pragma once

#include "../buffer.h"

// a smart pointer for PK_Verifier so less cleanup is needed if something throws
typedef std::auto_ptr<CryptoPP::PK_Verifier> PK_VerifierPtr;

// Signature Algorithm OIDs commonly used in certs that have RSA keys
DEFINE_OID(CryptoPP::ASN1::pkcs_1()+4, md5withRSAEncryption);
DEFINE_OID(CryptoPP::ASN1::pkcs_1()+5, sha1withRSAEncryption);

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
    ByteQueue googleq, thawteq, googletbs, thawtespki;
    SecByteBlock certSignature;

    void GetPublicKeyFromCert(CryptoPP::BufferedTransformation & certin, CryptoPP::BufferedTransformation & keyout)
    {
           BERSequenceDecoder x509Cert(certin);
           BERSequenceDecoder tbsCert(x509Cert);

           // ASN.1 from RFC 3280
           // TBSCertificate  ::=  SEQUENCE  {
           // version         [0]  EXPLICIT Version DEFAULT v1,

           // consume the context tag on the version
           BERGeneralDecoder context(tbsCert,0xa0);
           word32 ver;

           // only want a v3 cert
           BERDecodeUnsigned<word32>(context,ver,INTEGER,2,2);

           // serialNumber         CertificateSerialNumber,
           Integer serial;
           serial.BERDecode(tbsCert);

           // signature            AlgorithmIdentifier,
           BERSequenceDecoder signature(tbsCert);
           signature.SkipAll();

           // issuer               Name,
           BERSequenceDecoder issuerName(tbsCert);
           issuerName.SkipAll();

           // validity             Validity,
           BERSequenceDecoder validity(tbsCert);
           validity.SkipAll();

           // subject              Name,
           BERSequenceDecoder subjectName(tbsCert);
           subjectName.SkipAll();

           // subjectPublicKeyInfo SubjectPublicKeyInfo,
           BERSequenceDecoder spki(tbsCert);
           DERSequenceEncoder spkiEncoder(keyout);

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
        googleq.Put(this->getBuffer()->getPointerToBuffer(), this->getBuffer()->getLength());
        thawteq.Put(cert->getBuffer()->getPointerToBuffer(), cert->getBuffer()->getLength());
        try
        {
            GetPublicKeyFromCert(thawteq,thawtespki);
        }
        catch(std::exception &)
        {
            throw("Failed to extract the public key from the CA certificate.");
            //return false;
        }
        //Public key read from Thawte CA certificate.

        OID sigAlgOID;

        try
        {
               // first, extract the data that the signature covers
               BERSequenceDecoder x509Cert(googleq);
               BERSequenceDecoder tbsCert(x509Cert);
               DERSequenceEncoder tbsEnc(googletbs);
               tbsCert.TransferAllTo(tbsEnc);
               tbsEnc.MessageEnd();
               // find the algorithm used to sign the data
               BERSequenceDecoder sigAlg(x509Cert);
               sigAlgOID.BERDecode(sigAlg);
               sigAlg.SkipAll();
               // extract the actual signature
               uint8_t unused = 0;
               BERDecodeBitString(x509Cert, certSignature, unused);
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
               verifier = PK_VerifierPtr(new RSASS<PKCS1v15,CryptoPP::MD5>::Verifier(thawtespki));
               //cout << "Signature algorithm is RSA with MD5 hash." << endl;
        }
        else if(sigAlgOID == sha1withRSAEncryption())
        {
               verifier = PK_VerifierPtr(new RSASS<PKCS1v15,CryptoPP::SHA1>::Verifier(thawtespki));
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
               googletbs.TransferAllTo(vf);
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
            return true;
        }

        //Signature verification failed

        return false;
    }
};
