CXX = g++
CXXFLAGS = -O2 -std=c++11
LDFLAGS = -lpthread
WARNFLAGS = -Wall -pedantic -Wextra

GPU_INC = -I/usr/local/cuda/include
GPU_LDFLAGS = -lnvidia-ml

all: proginfo

proginfo:
	$(CXX) $(CXXFLAGS) $(GPU_INC) $(WARNFLAGS) \
		src/proginfo.cpp -o proginfo \
		$(LDFLAGS) $(GPU_LDFLAGS)

clean:
	rm -f proginfo
