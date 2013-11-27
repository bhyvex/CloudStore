#!/bin/bash


checkRoot(){
if [ ! $(id -u) = 0 ];then
    echo "need be root!,your id -u is :$(id -u)"
    exit;
fi
}

PREFIX=/usr
INCLUDE=$PREFIX/include
LIB=$PREFIX/lib
help(){
    echo "$0 install|uninstall"
}
check(){
    #verify
    ls $INCLUDE/leveldb
    ls -l $LIB/libleveldb.a $LIB/libmemenv.a
}
myinstall(){
    checkRoot
    cp libleveldb.a $LIB/libleveldb.a
    cp libmemenv.a $LIB/libmemenv.a
    cp -R include/leveldb $INCLUDE/leveldb
    check
}
myuninstall(){
    checkRoot
    if [ -d $INCLUDE/leveldb ] ;then
        rm -R $INCLUDE/leveldb
    fi;
    if [ -f $LIB/libleveldb.a ] ;then
        rm $LIB/libleveldb.a
        rm $LIB/libmemenv.a
    fi;
}
if [ ! -z $1 ];then
    if [ $1 = "-h" -o $1 = "-?" ];then
        help
        exit 0
    fi
    if [ $1 = "install" ];then
        myinstall 
        echo "Install done..."
        exit
    fi;
    if [ $1 = "uninstall" ] ;then
        myuninstall 
        echo "Uninstall done..."
        exit
    fi;
    if [ $1 = "check" ];then
        check
        exit;
    fi;
fi;

help
