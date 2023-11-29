#!/bin/bash

set -e
set -x

apachectl -DFOREGROUND &
/data/client -ca /data/test-ca.crt -cert /data/client.crt -key /data/client.key -addr localhost -port 443
