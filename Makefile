
all:
	g++ -O3 -o sux sux.cpp

install:
	strip sux
	mkdir -p $(DESTDIR)
	cp ./sux $(DESTDIR)
