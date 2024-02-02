# Compiler
CXX                 = g++

# Compiler flags
CXXFLAGS            = -std=c++11 -Wall -Wextra

# Libraries
LDFLAGS             = -lncurses -ljsoncpp

# Directories
ROOM_DIRECTORY      = src/room
CLIENT_DIRECTORY    = src/client
INTERFACE_DIRECTORY = src/interface
HEADERS_DIRECTORY   = headers

# Source files
ROOM_SRCS           = $(ROOM_DIRECTORY)/room.cpp $(ROOM_DIRECTORY)/client.cpp main.cpp
CLIENT_SRCS         = $(CLIENT_DIRECTORY)/client.cpp $(INTERFACE_DIRECTORY)/interface.cpp client.cpp
INTERFACE_SRCS      = $(INTERFACE_DIRECTORY)/interface.cpp interface_main.cpp

# Object files
ROOM_OBJS           = $(ROOM_SRCS:.cpp=.o)
CLIENT_OBJS         = $(CLIENT_SRCS:.cpp=.o)
INTERFACE_OBJS      = $(INTERFACE_SRCS:.cpp=.o)

# Executables
ROOM_TARGET         = server
CLIENT_TARGET       = client
INTERFACE_TARGET    = interface

# Build object files for both room and client
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(HEADERS_DIRECTORY)

# Build room (server)
room: $(ROOM_OBJS)
	$(CXX) $(CXXFLAGS) -o $(ROOM_TARGET) $(ROOM_OBJS) $(LDFLAGS)

# Build client
client: $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJS) $(LDFLAGS)

# Build interface
interface: $(INTERFACE_OBJS)
	$(CXX) $(CXXFLAGS) -o $(INTERFACE_TARGET) $(INTERFACE_OBJS) $(LDFLAGS)

# Clean rule for room
clean-room:
	rm -f $(ROOM_OBJS) $(ROOM_TARGET)

# Clean rule for client
clean-client:
	rm -f $(CLIENT_OBJS) $(CLIENT_TARGET)

# Clean rule for interface
clean-interface:
	rm -f $(INTERFACE_OBJS) $(INTERFACE_TARGET)

# Clean all
clean: clean-room clean-client clean-interface
