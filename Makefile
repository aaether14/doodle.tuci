CC			= g++
CFLAGS		= -c -o3 -Wall -std=c++14
LDFLAGS		= -lpthread -luuid
SOURCES		= server.cpp thread/thread.cpp tcp/tcpserver.cpp tcp/tcpstream.cpp logger/log_policies.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= server

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
