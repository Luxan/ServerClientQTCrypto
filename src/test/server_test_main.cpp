#include <QtTest/QtTest>
#include <QApplication>

#define TEST_CLASS(TestObject) {\
TestObject tc(#TestObject);\
status |= QTest::qExec(&tc, argc, argv);\
}

#include "../../include/test/test_shared/test_buffer.h"
#include "../../include/test/test_shared/test_buffer_spitter.h"
#include "../../include/test/test_shared/test_id_client.h"
#include "../../include/test/test_shared/test_id_message.h"
#include "../../include/test/test_shared/test_id_room.h"
#include "../../include/test/test_shared/test_package_buffer.h"
#include "../../include/test/test_shared/test_user.h"
#include "../../include/test/test_shared/test_user_credentials.h"
#include "../../include/test/test_shared/test_user_relations.h"

#include "../../include/test/test_shared/crypto/test_certificate.h"
#include "../../include/test/test_shared/crypto/test_certificate_authority.h"
#include "../../include/test/test_shared/crypto/test_cipher_rsa.h"
#include "../../include/test/test_shared/crypto/test_crypto.h"
#include "../../include/test/test_shared/crypto/test_hash.h"
#include "../../include/test/test_shared/crypto/test_hasher.h"
#include "../../include/test/test_shared/crypto/test_iv.h"
#include "../../include/test/test_shared/crypto/test_key.h"
#include "../../include/test/test_shared/crypto/test_key_agreement_agent.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int status = 0;

    TEST_CLASS(Test_Buffer)
    TEST_CLASS(Test_BufferSpitter)
    //-----------those 3 classes has dependenties so must be tested in proper order!
    TEST_CLASS(Test_ClientID)
    TEST_CLASS(Test_MessageID)
    TEST_CLASS(Test_RoomID)
//-----------
    TEST_CLASS(Test_PackageBuffer)
    TEST_CLASS(Test_PackageBuffer)
    TEST_CLASS(Test_UserCredentials)
    TEST_CLASS(Test_UserRelations)
//-------Crypto
    TEST_CLASS(Test_Certificate)
    TEST_CLASS(Test_CertificateAuthority)
    TEST_CLASS(Test_RSACipher)
    TEST_CLASS(Test_Crypto)
    TEST_CLASS(Test_Hash)
    TEST_CLASS(Test_Sha256Hasher)
    TEST_CLASS(Test_Iv)
    TEST_CLASS(Test_Key)
    TEST_CLASS(Test_KeyAgreamentAgent)
    return status;
}
