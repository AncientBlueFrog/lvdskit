install:
	cp lvds_error.h ~/.local/include/
	clang -fPIC -o lvdskit.o -c lvdskit.c
	clang -shared -fPIC -o liblvdskit.so lvdskit.o ../liblvdsLinkedList/lvdsllist.o ../liblvdsHashTable/lvdshtable.o ../liblvdsTrie/lvdstrie.o
	ar -rcs liblvdskit.a lvdskit.o ../liblvdsLinkedList/lvdsllist.o ../liblvdsHashTable/lvdshtable.o ../liblvdsTrie/lvdstrie.o
	cp liblvdskit.a ~/.local/lib/
	cp lvdskit.h ~/.local/include/
	echo "lvdskit installed"

uninstall:
	rm ~/.local/include/lvdskit.h
	rm ~/.local/lib/liblvdskit.a
	echo "lvdskit uninstalled"
