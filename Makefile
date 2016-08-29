#global conf
all:	client server test_client test_server run_test

rebuild: clean all

clean:	clean_client clean_server
gendoc:
	bash generate_documentation.sh

#client conf
client:	make_client clean_temp_client test_client

rebuild_client: clean_client client

clean_client: clean_temp_client clean_test_client
	test -d obj/Client && rm -rf obj/Client || echo ""
	test -d bin/Client && rm -rf bin/Client || echo ""

make_client:
	cd pro/Client/;	qmake Client.pro
	make -C pro/Client/

clean_temp_client:
	test -f pro/Client/Makefile && rm pro/Client/Makefile || echo ""
	test -f pro/Client/moc_mainwindow.cpp && rm pro/Client/moc_mainwindow.cpp || echo ""
	test -f pro/Client/ui_mainwindow.h && rm pro/Client/ui_mainwindow.h || echo ""
#----------------client tests---------------------------------------

test_client: clean_test_client make_test_client clean_temp_test_client test_client_run

test_client_run:
	bin/Test/Client/Test

clean_test_client: clean_temp_test_client
	test -d obj/Test/Client && rm -rf obj/Test/Client || echo ""
	test -d bin/Test/Client && rm -rf bin/Test/Client || echo ""
	
make_test_client:
	cd pro/Test/Client/Test; qmake Test.pro
	make -C pro/Test/Client/

clean_temp_test_client:
	test -f pro/Test/Client/Makefile && rm pro/Test/Client/Makefile || echo ""

#server conf
server:	make_server clean_temp_server test_server

rebuild_server: clean_server server

clean_server: clean_temp_server clean_test_server
	test -d obj/Server && rm -rf obj/Server || echo ""
	test -d bin/Server && rm -rf bin/Server || echo ""

make_server: 
	cd pro/Server/;	qmake Server.pro
	make -C pro/Server/

clean_temp_server:
	test -f pro/Server/Makefile && rm pro/Server/Makefile || echo ""
	test -f pro/Server/moc_mainwindow.cpp && rm pro/Server/moc_mainwindow.cpp || echo ""
	test -f pro/Server/ui_mainwindow.h && rm pro/Server/ui_mainwindow.h || echo ""

#----------------server tests---------------------------------------

test_server: clean_test_server make_test_server clean_temp_test_server test_server_run

test_server_run:
	bin/Test/Server/Test

clean_test_server: clean_temp_test_server
	test -d obj/Test/Server && rm -rf obj/Test/Server || echo ""
	test -d bin/Test/Server && rm -rf bin/Test/Server || echo ""
	
make_test_server:
	cd pro/Test/Server/Test; qmake Test.pro
	make -C pro/Test/Server/

clean_temp_test_server:
	test -f pro/Test/Server/Makefile && rm pro/Test/Server/Makefile || echo ""

