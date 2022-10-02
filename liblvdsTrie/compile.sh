clang -fPIC -o lvdstrie.o -c lvdstrie.c
clang -shared -fPIC -o liblvdstrie.so lvdstrie.o
ar -rcs liblvdstrie.a lvdstrie.o
cp liblvdstrie.a ~/lib/
cp lvdstrie.h ~/include/
