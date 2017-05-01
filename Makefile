CC			= g++
CFLAGS		= -c -Wall -std=c++14
LDFLAGS		= -lpthread
SOURCES		= server.cpp thread/thread.cpp tcp/tcpacceptor.cpp tcp/tcpstream.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= server

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
