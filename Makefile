
# ./make.zynq makes bin/pdmean directly
#
all: pdmean.x86
	echo Hello all

clean:
	rm bin/pdmean pdmean.x86

CFLAGS += -I../../include

bin/pdmean: pdmean.cpp
	${CC} ${CFLAGS} pdmean.cpp -o bin/pdmean -L../../lib/linux-arm -lpopt


pdmean.x86: pdmean.cpp
	cc pdmean.cpp -o pdmean.x86 -lpopt


	
	
