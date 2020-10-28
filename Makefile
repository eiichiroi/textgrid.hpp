CXX=g++
CXXFLAGS=-g -O2 -Wall -std=c++11

SOURCES= \
  textgrid.hpp \
  examples/textgrid2csv.cpp \
  examples/textgrid_merge.cpp \
  examples/textgrid_print.cpp
TARGETS= \
  examples/textgrid2csv \
  examples/textgrid_merge \
  examples/textgrid_print

GTEST_SOURCES= \
  tests/gtest/gtest.h \
  tests/gtest/gtest-all.cc \
  tests/gtest/gtest_main.cc
GTEST_TARGETS= \
  tests/gtest/libgtest.a \
  tests/gtest/libgtest_main.a
GTEST_OBJECTS= \
  tests/gtest/gtest-all.o \
  tests/gtest/gtest_main.o

TEST_SOURCES= \
  tests/textgrid_test.cpp
TEST_TARGETS= \
  tests/textgrid_test

PREFIX=/usr/local
INCLUDEDIR=$(PREFIX)/include
TESTDIR=tests/

build: examples

examples: $(TARGETS)

install:
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -p -m 0644 textgrid.hpp $(DESTDIR)$(INCLUDEDIR)

uninstall:
	$(RM) $(DESTDIR)$(INCLUDEDIR)/textgrid.hpp

check: build gtest $(TEST_TARGETS) check-tests

gtest: $(GTEST_SOURCES) $(GTEST_TARGETS)

tests/gtest/libgtest.a: tests/gtest/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

tests/gtest/gtest-all.o: tests/gtest/gtest-all.cc
	$(CXX) -I$(TESTDIR) $(CXXFLAGS) -pthread -c -o $@ $^

tests/gtest/libgtest_main.a: tests/gtest/gtest-all.o tests/gtest/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

tests/gtest/gtest_main.o: tests/gtest/gtest_main.cc
	$(CXX) -I$(TESTDIR) $(CXXFLAGS) -pthread -c -o $@ $^

tests/textgrid_test: tests/textgrid_test.cpp
	$(CXX) -I$(TESTDIR) $(CXXFLAGS) -pthread -o $@ $^ tests/gtest/libgtest_main.a

check-tests: $(TEST_TARGETS)
	for test in $(TEST_TARGETS); do $$test; done

check-style: check-style-by-cpplint

check-style-by-cpplint: $(SOURCES) $(TEST_SOURCES)
	./cpplint.py $(SOURCES) $(TEST_SOURCES)

check-style-by-clang-format: $(SOURCES) $(TEST_SOURCES)
	clang-format -i $(SOURCES) $(TEST_SOURCES)

clean:
	$(RM) $(TARGETS)
	$(RM) $(GTEST_OBJECTS) $(GTEST_TARGETS)
	$(RM) $(TEST_TARGETS)

.PHONY: build examples install uninstall clean \
        check gtest check-tests check-style check-style-by-cpplint check-style-by-clang-format
