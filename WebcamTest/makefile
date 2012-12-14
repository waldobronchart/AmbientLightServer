CFLAGS = -fpermissive

LIBS = -lpthread

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp) $(wildcard ../wiringPi/*.c)

webcam: $(SRC)
	g++ -o $@ $^ $(CFLAGS) $(LIBS) -DRASPI
