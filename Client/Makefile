CC =		g++
AR =		ar
#ARFLAGS =	@ARFLAGS@
INCLUDES =	-I. -I$(NDSE_SDK)/include
CFLAGS =	-g -O2 $(INCLUDES)
LDFLAGS = -lglut -L$(SOLUTION_DIR) -lKEYPAD_Actor -lVRAM_Reactor -lelf -lNDSE -ldwarf -ldistorm64 -lboost_thread

SRCS=Client.cpp



OBJS	:=	$(SRCS:.cpp=.o)

all: NDSE_Client

$(OBJS): %.o:
	$(CC) $(CFLAGS) -c $*.cpp -o $@

NDSE_Client: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o
	rm -f NDSE_Client






