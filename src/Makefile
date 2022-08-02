# UNIX makefile for mg_bun module (mg_bun.so)
#
# Build:
# make Makefile
#
# Cleanup:
# make clean
#
# Install:
# make install

MGBUNDIR=/usr/local/lib/mg_bun
MGBUNINC=./

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
   MGBUNOS = Linux
   CC = cc
   CCFLAGS += -fpic -DLINUX
   LINK += cc
   LINKFLAGS += -shared -rdynamic
endif
ifeq ($(UNAME_S),Darwin)
   MGBUNOS = OSX
   CC = cc
   CCFLAGS += -fPIC -fno-common -DMACOSX -D_NOTHREADS -DDARWIN
   LINK += cc
   LINKFLAGS += -bundle -flat_namespace -undefined suppress
endif
ifeq ($(UNAME_S),FreeBSD)
   MGBUNOS = FreeBSD
   CC = cc
   CCFLAGS += -DFREEBSD
   LINK = ld
   LINKFLAGS += -G
endif
ifeq ($(UNAME_S),AIX)
   MGBUNOS = AIX
   CC = xlc_r
   CCFLAGS += -DAIX
   LINK = xlc_r
   LINKFLAGS += -G -H512 -T512 -bM:SRE -berok -bexpall -bnoentry
endif

ifndef MGBUNOS
   $(error Cannot determine the OS type)
endif

# Build mg_bun module
all : app

# Link the object file and dependent libraries into a binary
app : mg_dba.o mg_bun.o
	$(LINK) -o mg_bun.so $(LINKFLAGS) mg_dba.o mg_bun.o

# Compile the source file into object file
mg_bun.o : mg_bun.c
	$(CC) -o mg_bun.o $(CCFLAGS) -DMG_BUN_DSO -I$(MGBUNINC) -c mg_bun.c

mg_dba.o : mg_dba.c
	$(CC) -o mg_dba.o $(CCFLAGS) -DMG_BUN_DSO -I$(MGBUNINC) -c mg_dba.c

# Install mg_bun module
install :
	cp mg_bun.so $(MGBUNDIR)/mg_bun.so
	cp mg_bun.ts $(MGBUNDIR)/mg_bun.ts

# Clean target
clean :
	rm -rf mg_bun.so mg_bun.o mg_dba.o
