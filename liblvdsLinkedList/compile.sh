clang -fPIC -o lvdsllist.o -c lvdsllist.c
clang -shared -fPIC -o liblvdsllist.so lvdsllist.o
ar -rcs liblvdsllist.a lvdsllist.o
cp liblvdsllist.a ~/lib/
cp lvdsllist.h ~/include/
