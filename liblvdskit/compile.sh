#!/bin/zsh
clang -fPIC -o lvdskit.o -c lvdskit.c
clang -shared -fPIC -o liblvdskit.so lvdskit.o ../liblvdsLinkedList/lvdsllist.o ../liblvdsHashTable/lvdshtable.o ../liblvdsTrie/lvdstrie.o
ar -rcs liblvdskit.a lvdskit.o ../liblvdsLinkedList/lvdsllist.o ../liblvdsHashTable/lvdshtable.o ../liblvdsTrie/lvdstrie.o
cp liblvdskit.a ~/lib/
cp lvdskit.h ~/include/
