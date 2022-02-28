# appointed libs and their dir
LOCAL_SHARED_LIB:= akuio akispsdk akv_encode ak_mt
CUR_DIR:=$(shell pwd)

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

CFLAGS += -std=c99 -mlittle-endian  -fno-builtin -nostdlib -O2 -mlong-calls

ifndef PLAT_INC_DIR
    PLAT_INC_DIR=$(CUR_DIR)/../../../libplat/include
	CFLAGS += -I$(PLAT_INC_DIR)
endif

ifndef PLAT_INC_DIR_INNER
    PLAT_INC_DIR_INNER=$(CUR_DIR)/../../../libplat/include_inner
	CFLAGS += -I$(PLAT_INC_DIR_INNER)
endif

ifndef MPI_INC_DIR
    MPI_INC_DIR=$(CUR_DIR)/../../include
	CFLAGS += -I$(MPI_INC_DIR)
endif

ifdef AK_RTOS
	CFLAGS += -I../../../../kernel/include
endif

ifeq ($(COMPILE_SO), y)
LOCAL_SHARED_LIB+= plat_common plat_thread plat_vi plat_vpss plat_ipcsrv plat_venc_cb plat_drv
MPI_SHARED_LIB:= mpi_venc
else
PLAT_STATIC_LIB:= plat_common plat_thread plat_vi plat_vpss plat_ipcsrv plat_venc_cb plat_drv
MPI_STATIC_LIB:= mpi_venc
endif

ifeq ($(DEBUG), y)
	PLAT_STATIC_LIB += dmallocthcxx dmallocth
endif

LIBS := $(addprefix $(PLAT_LIB_DIR)/,$(patsubst %,lib%.a,$(PLAT_STATIC_LIB)))
LIBS+= $(addprefix $(MPI_LIB_DIR)/,$(patsubst %,lib%.a,$(MPI_STATIC_LIB)))

LDFLAGS := $(LOCAL_LDFLAGS)
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(LOCAL_SHARED_LIB)))
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(MPI_SHARED_LIB)))
LDFLAGS += $(STD_LDFLAGS)

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

ifndef AK_RTOS
TARGET := ak_mt_demo

all:$(TARGET)

$(TARGET):$(OBJS)
	$(LDCXX) $^ $(LDFLAGS) -Xlinker "-(" $(LIBS) -Xlinker "-)" -o $@
ifeq ($(DEBUG), n)
	$(STRIP) $@
endif
else
endif


#%.o : %.c
$(OBJS) : $(SRCS)
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo ""
ifdef AK_RTOS
	$(CP) $@ ../../../../kernel/objs/
endif

ifndef AK_RTOS
install:
	@$(CP) $(TARGET) $(ROOTFS_BIN_DIR)
	@echo ""
endif

clean:
	$(RM) *.o
	$(RM) $(TARGET)

.PHONY: all clean install
