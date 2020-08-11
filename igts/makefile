DEBUG = 1

ifeq ($(DEBUG),1)
	DEBFLAGS = -g  -Wuninitialized -O0 -D__DEBUG
else
	DEBFLAGS = -s -O2
endif

CC = g++

INC = -I../include  -I./ -I./threadpool
CFLAGS = -DLINUX -DUNIX $(DEBFLAGS) $(INC) -std=c++11

LDFLAGS = -L../lib
LIBS = -lm -lpthread 
BIN = server


DIR := $(shell find . -maxdepth 3 -type d)
#将DIR中的值取出来（空格区分），赋值给dir，然后执行$(wildcard $(dir)/*.cpp)
CPP_SRCS = $(foreach dir,$(DIR),$(wildcard $(dir)/*.cpp))
C_SRCS = $(foreach dir,$(DIR),$(wildcard $(dir)/*.c))


C_OBJS = $(patsubst %.c,%.o,$(C_SRCS))
CPP_OBJS = $(patsubst %.cpp,%.o,$(CPP_SRCS))

SRCS = $(C_SRCS) $(CPP_SRCS)
OBJS = $(C_OBJS) $(CPP_OBJS)



.PHONY: all clean  install test

%.o:%.c
	gcc -c $< -o $@ $(CFLAGS) $(LDFLAGS) $(LIBS)
%.o:%.cpp
	g++ -c $< -o $@ $(CFLAGS) $(LDFLAGS) $(LIBS)

all: $(BIN) 

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(CFLAGS) $(LIBS) 

clean:
	@-rm $(BIN) -f
	@-rm *.o -f

test:
	@echo "***************  printf info begin********************"
	@echo "CC" = $(CC) 
	@echo "BIN" = $(BIN) 
	@echo "OBJS" = $(OBJS) 
	@echo "LDFLAGS" = $(LDFLAGS) 
	@echo "CFLAGS" = $(CFLAGS) 
	@echo "LIBS" = $(LIBS) 	
	@echo " if use libevent,add macro USELIBEVENTG !"
	@echo "***************  printf info end ********************"





