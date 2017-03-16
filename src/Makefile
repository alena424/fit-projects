# Path to gtest root
GTEST_DIR = ./googletest/googletest

# Path to our programs
SRC_DIR = .

# Flags for the preprocessor
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags for the C++ compiler
CXXFLAGS += -g -Wall -Wextra -pthread

# Name of the tests
TESTS = tdd_tests

# Google test headers
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h

# Build targets

test : $(TESTS)

downloadgtest :
	git clone https://github.com/google/googletest/

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

# Setting sources
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_DIR) $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
		$(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

AteamMathLib.o : $(SRC_DIR)/AteamMathLib.h $(SRC_DIR)/AteamMathLib.cpp $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/AteamMathLib.cpp

tdd_tests.o : $(SRC_DIR)/tdd_tests.cpp $(SRC_DIR)/AteamMathLib.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC_DIR)/tdd_tests.cpp

tdd_tests : AteamMathLib.o tdd_tests.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(GTEST_HEADERS) :
	$(error GoogleTest not found. Please run "make downloadgtest" first)
