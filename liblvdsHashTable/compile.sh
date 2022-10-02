clang -fPIC -o lvdshtable.o -c lvdshtable.c
clang -shared -fPIC -o liblvdshtable.so lvdshtable.o
ar -rcs liblvdshtable.a lvdshtable.o
cp liblvdshtable.a ~/lib/
cp lvdshtable.h ~/include/
