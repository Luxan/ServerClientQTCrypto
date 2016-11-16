#!/bin/bash
COUNTRY="PL"
STATE="Malopolskie"
CITY="Krakow"
COMPANY="gorokh"
ORGANIZATIONUNIT="MESSANGER"
CACOMMONNAME="CA MESSANGER"
SERVERCOMMONNAME="SERVER MESSANGER"
PASSWORD="foobar"

#if [ ! -f ca.cert ]; then
	touch certindex
	echo "" > certindex
	echo 01 > certserial
	echo 01 > crlnumber
#fi 

openssl genrsa -out ca.key 4096

openssl req -new -x509 -days 1826 -key ca.key -out ca.crt

openssl genrsa -out ia.key 4096

openssl req -new -key ia.key -out ia.csr

openssl ca -batch -config conf/ca.conf -notext -in ia.csr -out ia.crt
