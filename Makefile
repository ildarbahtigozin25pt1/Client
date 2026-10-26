CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = vclient
SRCS = main.cpp interface.cpp authenticator.cpp network.cpp file_io.cpp client.cpp
OBJS = $(SRCS:.cpp=.o)

LIBS = -lboost_program_options -lcryptopp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
