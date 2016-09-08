#################[Variables]#################
this_dir = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
#################[Global]#################
all:	is_lib_installed client test_client server test_server
rebuild: clean all
clean:	clean_client clean_server
gendoc:
	bash generate_documentation.sh
#################[Lib]#################
is_lib_installed: is_crypto++_installed
lib: crypto++
###[Crypto++]###
rebuild_crypto++: crypto++clean crypto++
crypto++:
	cd lib/crypto++/src; make
	cd lib/crypto++/src; make test
	cd lib/crypto++/src; make libcryptopp.so
	cd lib/crypto++/src; make install PREFIX=$(this_dir)lib/crypto++/installed
crypto++clean:
	cd lib/crypto++/src; make clean
	$(RM) -rf lib/crypto++/installed
is_crypto++_installed:
	test -d lib/crypto++/installed && echo "Library crypto++ is already installed." || $(MAKE) crypto++
#################[Client]#################
client:	is_lib_installed make_client clean_temp_client test_client
rebuild_client: clean_client client
clean_client: clean_temp_client clean_test_client
	$(RM) -rf obj/Client
	$(RM) -rf bin/Client
make_client:
	cd pro/Client/;	qmake Client.pro
	make -C pro/Client/
clean_temp_client:
	$(RM) pro/Client/Makefile
	$(RM) pro/Client/*.cpp
	$(RM) pro/Client/*.h
###[Client]###
test_client: clean_test_client make_test_client clean_temp_test_client test_client_run
test_client_run:
	bin/Test/Client/Test
clean_test_client: clean_temp_test_client
	$(RM) -rf obj/Test/Client
	$(RM) -rf bin/Test/Client
make_test_client:
	cd pro/Test/Client/Test; qmake Test.pro
	$(MAKE) -C pro/Test/Client/
clean_temp_test_client:
	$(RM) pro/Test/Client/Makefile
#################[Server]#################
server:	is_lib_installed make_server clean_temp_server test_server
rebuild_server: clean_server server
clean_server: clean_temp_server clean_test_server
	$(RM) -rf obj/Server
	$(RM) -rf bin/Server
make_server: 
	cd pro/Server/;	qmake Server.pro
	$(MAKE) -C pro/Server/
clean_temp_server:
	$(RM) pro/Server/Makefile
	$(RM) pro/Server/moc_mainwindow.cpp
	$(RM) pro/Server/ui_mainwindow.h
###[Server test]###
test_server: clean_test_server make_test_server clean_temp_test_server test_server_run
test_server_run:
	bin/Test/Server/Test
clean_test_server: clean_temp_test_server
	$(RM) -rf obj/Test/Server
	$(RM) -rf bin/Test/Server	
make_test_server:
	cd pro/Test/Server/Test; qmake Test.pro
	$(MAKE) -C pro/Test/Server/
clean_temp_test_server:
	$(RM) pro/Test/Server/Makefile
