#include <QtTest/QtTest>
#include <QApplication>

#define TEST_CLASS(TestObject) {\
TestObject tc;\
status |= QTest::qExec(&tc, argc, argv);\
}

#include "../../include/test/test_shared/test_buffer.h"
#include "../../include/test/test_shared/test_buffer_spitter.h"
#include "../../include/test/test_shared/test_id_client.h"
#include "../../include/test/test_shared/test_id_message.h"
#include "../../include/test/test_shared/test_id_room.h"
#include "../../include/test/test_shared/test_package_buffer.h"
#include "../../include/test/test_shared/test_user.h"


//#include "../../obj/Test/Server/moc/test_buffer.moc"
//#include "../../obj/Test/Server/moc/test_buffer_spitter.moc"
//#include "../../obj/Test/Server/moc/test_id_client.moc"
//#include "../../obj/Test/Server/moc/test_id_message.moc"
//#include "../../obj/Test/Server/moc/test_id_room.moc"
//#include "../../obj/Test/Server/moc/test_package_buffer.moc"
//#include "../../obj/Test/Server/moc/test_user.moc"


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
    TEST_CLASS(Test_User)

    return status;
}
