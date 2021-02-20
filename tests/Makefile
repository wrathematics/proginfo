CXX=g++
CXXFLAGS = -O2 -std=c++11
LDFLAGS = -lpthread
WARNFLAGS = -Wall -pedantic -Wextra

GPU_INC = -I/usr/local/cuda/include
GPU_LDFLAGS = -lnvidia-ml


all: proginfo

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o )

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(WARNFLAGS) $(GPU_INC) -c $< -o $@

proginfo: $(OBJS)
	$(CXX) $(OBJS) -o proginfo $(LDFLAGS) $(GPU_LDFLAGS)

miniclean:
	( ls *.o | grep -v test-main | xargs rm -f )

clean:
	rm -rf *.o proginfo
