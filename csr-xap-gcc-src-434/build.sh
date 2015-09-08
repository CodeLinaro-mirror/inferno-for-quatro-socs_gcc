#!/bin/sh

set -x
set -e

tar xzf gcc-core-4.3.4.tar.gz
tar xzf gmp-4.3.1.tar.gz
tar xzf mpfr-2.4.2.tar.gz
tar xjf binutils-2.17.tar.bz2

mv gcc-4.3.4 gcc
mv gmp-4.3.1 gcc/gmp
mv mpfr-2.4.2 gcc/mpfr
mv binutils-2.17 xapbinutils

patch -p6 <gcc-xap.diff
tar xzf gcc-extra.tgz

patch -p5 <binutils-xap.diff
tar xzf binutils-extra.tgz

INSTALL_GCC=`pwd`/install/gcc
INSTALL_BINUTILS=`pwd`/install/binutils

mkdir -p $INSTALL_GCC
mkdir -p $INSTALL_BINUTILS 
mkdir -p build/binutils build/gcc

pushd build/binutils
../../xapbinutils/configure \
    --enable-werror=no --target=xap2 --prefix=$INSTALL_BINUTILS
make -sj4
make -s install
popd

mkdir -p install/gcc/libexec/gcc/xap-local-xap/4.3.4
mkdir -p install/gcc/bin
cp install/binutils/xap2/bin/as install/gcc/libexec/gcc/xap-local-xap/4.3.4
cp install/binutils/xap2/bin/ld install/gcc/libexec/gcc/xap-local-xap/4.3.4
cp install/binutils/xap2/bin/ar install/gcc/bin/xap-local-xap-ar
cp install/binutils/xap2/bin/ranlib install/gcc/bin/xap-local-xap-ranlib

pushd build/gcc
export TCFLAGS=-mas-mode
export PATH=$INSTALL_GCC/bin:$PATH
../../gcc/configure \
    --enable-languages=c --target=xap-local-xap --disable-libssp \
    --disable-shared --prefix=$INSTALL_GCC
make -sj4
make install
popd

