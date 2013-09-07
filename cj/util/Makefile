CC=gcc

PROJ=libconv.a
OBJS=conv_comm.o gbk_to_utf8.o utf8_to_gbk.o

CCFLAGS =  -D__LINUX__

ifndef DEBUG_SET
#DEBUG_SET=-g
endif


all: $(PROJ)

$(PROJ):$(OBJS)
	@(ar -rcs $@ $^)
	@(echo "create libtconv.a success")
	
.cpp.o:
	$(CC) $(DEBUG_SET) $(CCFLAGS) $(INCLUDES) -fPIC -c $<
	
.c.o:
	$(CC) $(DEBUG_SET) $(CCFLAGS) $(INCLUDES) -fPIC -c $<