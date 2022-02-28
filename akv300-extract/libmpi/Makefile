CUR_DIR = $(shell pwd)

ifndef MPI_LIB_DIR
	MPI_LIB_DIR=$(CUR_DIR)/lib
	export MPI_LIB_DIR
endif

ifndef CROSS_COMPILE
	CROSS_COMPILE ?= arm-anykav200-linux-uclibcgnueabi-
	CC := $(CROSS_COMPILE)gcc
	CXX := $(CROSS_COMPILE)g++ 
	LD:=$(CC)
	LDCC:=$(CC)
	LDCXX:=$(CXX)
	STRIP:=$(CROSS_COMPILE)strip
	AR:=$(CROSS_COMPILE)ar
	ARFLAGS:= crs
	MAKE:=make
	CP:=cp -f
	RM:=rm -rf
	export CROSS_COMPILE CC CXX LD LDCC LDCXX STRIP CXXFLAGS AR ARFLAGS MAKE CP RM
endif

ifndef PLAT_INC_DIR
	PLAT_INC_DIR=$(CUR_DIR)/../libplat/include
endif

ifndef PLAT_INC_DIR_INNER
	PLAT_INC_DIR_INNER=$(CUR_DIR)/../libplat/include_inner
endif

ifndef MPI_INC_DIR
	MPI_INC_DIR=$(CUR_DIR)/include
endif

ifndef CFLAGS
#	CFLAGS :=-fno-strict-aliasing -Os -Wall -Werror -D_GNU_SOURCE -std=c99 -fms-extensions 
	CFLAGS :=-fno-strict-aliasing -Os -Wall -D_GNU_SOURCE -std=c99 -fms-extensions 
endif

CFLAGS += $(addprefix -I,$(PLAT_INC_DIR))
CFLAGS += $(addprefix -I,$(PLAT_INC_DIR_INNER))
CFLAGS += $(addprefix -I,$(MPI_INC_DIR))

ifdef AK_RTOS
	CFLAGS += $(AK_RTOS)
endif

export CFLAGS

dir_y := 
ifndef PLAT_RELEASE
	PLAT_RELEASE=n
endif

ifeq ($(PLAT_RELEASE), n)
	dir_y += src
endif
dir_y += lib

ifndef COMPILE_SO
	COMPILE_SO=y
	export COMPILE_SO
endif

ifeq ($(COMPILE_SO), n)
	LOCAL_COMMON_FLAG := $(ARFLAGS)
	LOCAL_GEN := $(AR)
else
	LOCAL_COMMON_FLAG := -shared -fPIC -o
	LOCAL_GEN := $(CC)
endif

export LOCAL_GEN LOCAL_COMMON_FLAG

ifeq ($(COMPILE_DEMO), y)
	dir_y += demo
endif

all:
	@for i in $(dir_y); \
	do \
		$(MAKE) -C $$i; \
		if [ $$? -ne 0 ];then exit 1; fi \
	done

install:
	@for i in $(dir_y); \
	do \
		$(MAKE) -C $$i install; \
	done

clean:
	@for i in $(dir_y); \
	do \
		$(MAKE) -C $$i clean; \
	done

image:

demo:
	$(MAKE) -C demo

.PHONY: all install clean demo
