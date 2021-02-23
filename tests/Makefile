-include make.inc

CXXFLAGS = -O2 -std=c++11
LDFLAGS = -lpthread
WARNFLAGS = -Wall -pedantic -Wextra


all: proginfo

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o )

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(WARNFLAGS) -c $< -o $@

proginfo: $(OBJS)
	$(CXX) $(OBJS) -o proginfo $(LDFLAGS)

miniclean:
	( ls *.o | grep -v test-main | xargs rm -f )

clean:
	rm -rf *.o proginfo
