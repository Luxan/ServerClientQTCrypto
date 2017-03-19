#/bin/bash

cd CA

openssl req -nodes -new -x509 -keyout private/serverkey.pem -out serverreq.pem -days 365 -config ca.conf

openssl x509 -x509toreq -in serverreq.pem -signkey private/serverkey.pem -out tmp.pem

openssl ca -config ca.conf -policy policy_anything -out servercert.pem -infiles tmp.pem

rm tmp.pem

cd ..
