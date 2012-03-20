lib_LTLIBRARIES = liboll.la
liboll_la_CPPFLAGS = -D_REENTRANT
liboll_la_LIBADD = -lz
liboll_la_SOURCES = \
  oll.hpp oll.cpp

pkginclude_HEADERS = oll.hpp

bin_PROGRAMS = oll_train oll_test oll_line

oll_train_CPPFLAGS = -D_REENTRANT
oll_train_LDADD    = liboll.la
oll_train_SOURCES  = oll_train.cpp

oll_test_CPPFLAGS  = -D_REENTRANT
oll_test_LDADD     = liboll.la
oll_test_SOURCES   = oll_test.cpp

oll_line_CPPFLAGS  = -D_REENTRANT
oll_line_LDADD     = liboll.la
oll_line_SOURCES   = oll_line.cpp
