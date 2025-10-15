
# ./make.zynq makes bin/pdmean directly
#
all: pdmean.x86
	echo Hello all

clean:
	rm bin/pdmean pdmean.x86

CFLAGS += -I../../include -O2

bin/pdmean: pdmean.cpp
	${CXX} ${CFLAGS} pdmean.cpp -o bin/pdmean -L../../lib/linux-arm -lpopt


pdmean.x86: pdmean.cpp
	${CXX} pdmean.cpp -o pdmean.x86 -lpopt


	
	
