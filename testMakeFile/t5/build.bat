echo off

echo 'start cmake in windows'

call vcvarsall x86

cmake -Bbuild -G "NMake Makefiles"

cd build

nmake

echo 'end all'