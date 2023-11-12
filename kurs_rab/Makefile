all:
	g++ main.cpp ServerInteraction.cpp ./md5/md5.cpp -o client -lcryptopp

utest:
	g++ UnitTest.cpp ServerInteraction.cpp ./md5/md5.cpp -o utest -lcryptopp -lUnitTest++ 
clean:
	rm -f client
