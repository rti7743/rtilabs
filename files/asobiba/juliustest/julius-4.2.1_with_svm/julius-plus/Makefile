LIBSENT=../libsent
LIBJULIUS=../libjulius

CXX=g++
CFLAGS=-g -O2

####
#### When using system-installed libraries
####
# CPPFLAGS=`libjulius-config --cflags` `libsent-config --cflags`
# LDFLAGS=`libjulius-config --libs` `libsent-config --libs`

####
#### When using within-package libraries
####
CPPFLAGS=-std=gnu++0x -Wwrite-strings -I$(LIBJULIUS)/include -I$(LIBSENT)/include  `$(LIBSENT)/libsent-config --cflags` `$(LIBJULIUS)/libjulius-config --cflags`
LDFLAGS=-lbfd -lboost_thread -lboost_system -L$(LIBJULIUS) `$(LIBJULIUS)/libjulius-config --libs` -L$(LIBSENT) `$(LIBSENT)/libsent-config --libs`

TARGET = julius-plus
OBJ = \
main.o \
JuliusPlus.o \
JuliusPlusRule.o \
ActionImplement.o \
liblinear/linear.o \
liblinear/tron.o \
liblinear/blas/daxpy.o \
liblinear/blas/ddot.o \
liblinear/blas/dnrm2.o \
liblinear/blas/dscal.o


############################################################

all: liblinear $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJ) $(LDFLAGS) 

liblinear:
	cd liblinear
	make

clean:
	(cd liblinear && make clean && cd .. )
	$(RM) *.o *.bak *~ core TAGS

distclean:
	(cd liblinear && make clean && cd .. )
	$(RM) *.o *.bak *~ core TAGS
	$(RM) julius-plus

