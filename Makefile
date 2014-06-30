# A generic template Makefile
# Author Red_Liu lli_njupt@163.com v0.1
#
# This file is a(part of) free software; you can redistribute it 
# and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2, or 
# (at your option) any later version.

.PHONY = all install clean

#global directory defined
TOPDIR       = $(shell pwd)
SRCDIR	 	= $(TOPDIR)/src
LIBDIR      = $(TOPDIR)/lib
OBJECTDIR    = $(TOPDIR)/build
INCLUDEDIR   = $(TOPDIR)/include

#cross compile tools defined 
CROSS_COMPILE ?= arm-linux-
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CC) -E
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
RANLIB 	= $(CROSS_COMPILE)ranlib

#local host tools defined
CP		:= cp
RM		:= rm
MKDIR	:= mkdir
SED		:= sed
FIND	:= find
MKDIR	:= mkdir
XARGS	:= xargs

#target name
TARGETMAIN  = lcd_check
TARGETLIBS 	= libmk.a
TARGETSLIBS = libmk.so

#FILE' INFOMATION COLLECT
VPATH 			= $(shell ls -AxR $(SRCDIR)|grep ":"|grep -v "\.svn"|tr -d ':')
SOURCEDIRS	= $(VPATH)

#search source file in the current dir
SOURCES  	= $(foreach subdir,$(SOURCEDIRS),$(wildcard $(subdir)/*.c))
SRCOBJS 			= $(patsubst %.c,%.o,$(SOURCES))
BUILDOBJS = $(subst $(SRCDIR),$(OBJECTDIR),$(SRCOBJS))
DEPS			= $(patsubst %.o,%.d,$(BUILDOBJS))

#external include file define
CFLAGS	= -O2 -Wall -MD $(foreach dir,$(INCLUDEDIR),-I$(dir))
ARFLAGS = rc

#special parameters for apps
CFLAGS	+=

#c file compile parameters and linked libraries
CPPFLAGS    = 
LDFLAGS	    =
XLDFLAGS   = -Xlinker "-(" $(LDFLAGS) -Xlinker "-)"
LDLIBS		 += -L $(LIBDIR) 

#defaut target:compile the currrent dir file and sub dir 
all: head $(TARGETMAIN)

#for .h header files dependence
-include $(DEPS)

$(TARGETMAIN) :$(BUILDOBJS)
	$(CC) $(subst $(SRCDIR),$(OBJECTDIR),$^) $(CPPFLAGS) $(CFLAGS) $(XLDFLAGS) -o $@ $(LDLIBS) 
#	$(STRIP)  --strip-unneeded $(TARGETMAIN)
	cp $(TARGETMAIN) /home/barnard/work/board_9G25/rootfs/root/project/bin

head:
	cp /home/barnard/work/board_9G25/linux-at91/drivers/spi/st7735/st7735.h  ./src/gui/

$(TARGETLIBS) :$(BUILDOBJS)
	$(AR) $(ARFLAGS) $@ $(BUILDOBJS)
	$(RANLIB) $@

$(TARGETSLIBS) :$(BUILDOBJS)
	$(CC) -shared $(subst $(SRCDIR),$(OBJECTDIR),$^) $(CPPFLAGS) $(CFLAGS) $(XLDFLAGS) -o $@ $(LDLIBS)

$(OBJECTDIR)%.o: $(SRCDIR)%.c
	[ ! -d $(dir $(subst $(SRCDIR),$(OBJECTDIR),$@)) ] & $(MKDIR) -p $(dir $(subst $(SRCDIR),$(OBJECTDIR),$@))
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $(subst $(SRCDIR),$(OBJECTDIR),$@) -c $<

intall:

clean:
	$(FIND) $(OBJECTDIR) -name "*.o" -o -name "*.d" | $(XARGS) $(RM) -f
	$(RM) -f $(TARGETMAIN) $(TARGETLIBS) $(TARGETSLIBS)

.PHONY:all clean copy head
