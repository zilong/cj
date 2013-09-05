CC=g++

PROJ=libbase.so
OBJS=Exception.o

CCFLAGS =  -D__LINUX__

ifndef DEBUG_SET
#DEBUG_SET=-g
endif

INCLUDES= -I ../hdr\
          -I${BOOST_ROOT}/include\

LDADD=   -L${BOOST_ROOT}/lib\

LIBS= 	-lpthread\
	 
all: $(PROJ)

$(PROJ):$(OBJS)
		$(CC) -shared -fPIC -o ./$(PROJ) $(OBJS)  $(LDADD) $(LIBS)

.cpp.o:
		$(CC) $(DEBUG_SET) $(CCFLAGS) $(INCLUDES) -fPIC -c $<

clean:
		rm *.o -f
		rm -rf ./$(PROJ)
		touch *
