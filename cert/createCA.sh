#/bin/bash

if [ ! -d "$CA" ]; then
	mkdir CA
fi

if [ ! -d "$CA/certs" ]; then
	mkdir CA/certs
fi

if [ ! -d "$CA/crl" ]; then
	mkdir CA/crl
fi

if [ ! -d "$CA/newcerts" ]; then
	mkdir CA/newcerts
fi

if [ ! -d "$CA/private" ]; then
	mkdir CA/private
fi

echo "01" > CA/serial

cp /dev/null CA/index.txt
cp ca.conf CA/ca.conf

openssl req -new -x509 -keyout CA/private/cakey.pem -out CA/cacert.pem -days 365 -config CA/ca.conf

