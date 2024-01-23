# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Libraries
LDFLAGS = -lncurses -ljsoncpp

# Directories
ROOM_DIRECTORY = src/room
CLIENT_DIRECTORY = src/client
HEADERS_DIRECTORY = headers

# Source files
ROOM_SRCS = main.cpp $(ROOM_DIRECTORY)/room.cpp $(ROOM_DIRECTORY)/client.cpp
CLIENT_SRCS = client.cpp $(CLIENT_DIRECTORY)/client.cpp

# Object files
ROOM_OBJS = $(ROOM_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

# Executables
ROOM_TARGET = server
CLIENT_TARGET = client

# Build object files for both room and client
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(HEADERS_DIRECTORY)

# Build room (server)
room: $(ROOM_OBJS)
	$(CXX) $(CXXFLAGS) -o $(ROOM_TARGET) $(ROOM_OBJS) $(LDFLAGS)

# Build client
client: $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJS) $(LDFLAGS)

# Clean rule for room
clean-room:
	rm -f $(ROOM_OBJS) $(ROOM_TARGET)

# Clean rule for client
clean-client:
	rm -f $(CLIENT_OBJS) $(CLIENT_TARGET)
