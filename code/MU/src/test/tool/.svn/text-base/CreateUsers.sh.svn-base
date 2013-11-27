#!/bin/bash

usage() {
    echo "$0 EXEC_NAME RS_IP RS_PORT MU_PORT FIRST_UID USERS"
}

if [ $# != 6 ]
then
    usage
    exit 1
fi

EXEC_NAME=$1
RS_IP=$2
RS_PORT=$3
MU_PORT=$4
FIRST_UID=$5
USERS=$6

for i in $(seq ${USERS})
do
    let uid=${FIRST_UID}+${i}
    ${EXEC_NAME} ${RS_IP} ${RS_PORT} ${MU_PORT} ${uid}
    sleep 1
done
