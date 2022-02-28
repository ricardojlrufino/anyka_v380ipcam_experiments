# appointed libs and their dir
LOCAL_SHARED_LIB:= akaudiofilter akaudiocodec

ifeq ($(COMPILE_SO), y)
LOCAL_SHARED_LIB+= plat_ai plat_common plat_thread plat_ipcsrv
MPI_SHARED_LIB:= mpi_aenc
else
PLAT_STATIC_LIB:= plat_ai plat_common plat_thread plat_ipcsrv
MPI_STATIC_LIB:= mpi_aenc
endif

LOCAL_LDFLAGS += $(addprefix $(PLAT_LIB_DIR)/,$(patsubst %,lib%.a,$(PLAT_STATIC_LIB)))
LOCAL_LDFLAGS += $(addprefix $(MPI_LIB_DIR)/,$(patsubst %,lib%.a,$(MPI_STATIC_LIB)))

LDFLAGS := $(LOCAL_LDFLAGS)
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(LOCAL_SHARED_LIB)))
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(MPI_SHARED_LIB)))

LDFLAGS += $(STD_LDFLAGS)

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
TARGET := ak_aenc_demo

all:$(TARGET)
	
$(TARGET):$(OBJS)
	$(LD) $^ $(LDFLAGS) -o $@
	$(STRIP) $@
	
%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo ""

install:
	@$(CP) $(TARGET) $(ROOTFS_BIN_DIR)
	@echo ""

clean:
	$(RM) *.o
	$(RM) $(TARGET)

.PHONY: all clean install
