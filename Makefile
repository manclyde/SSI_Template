CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=gnu++11

OBJS =		test.o

LIBS =

TARGET =	test

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
