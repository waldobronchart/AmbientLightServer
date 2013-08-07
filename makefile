CFLAGS = -fpermissive -L/usr/lib -Wwrite-strings -DRASPBERRY_PI

LIBS = -lopencv_core \
	-lopencv_highgui \
	-lpthread \
	-llog4cplus \
	-ljansson \
	-lboost_system \
	-lboost_date_time \
	-lboost_iostreams \
	-lboost_thread \
	-lboost_timer \
	-lwiringPi

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.cpp)

AmbiLightServer: $(SRC)
	g++ -o build/$@ $^ $(CFLAGS) $(LIBS)

clean:
	@rm -f AmbiLightServer *.o
