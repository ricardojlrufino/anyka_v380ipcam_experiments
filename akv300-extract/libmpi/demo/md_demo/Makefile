# appointed libs and their dir
LOCAL_SHARED_LIB:= akuio akispsdk akv_encode

ifeq ($(COMPILE_SO), y)
LOCAL_SHARED_LIB+= plat_common plat_thread plat_vi plat_vpss plat_ipcsrv plat_venc_cb
MPI_SHARED_LIB:= mpi_venc mpi_md
else
PLAT_STATIC_LIB:= plat_vi plat_common plat_thread plat_vpss plat_ipcsrv plat_venc_cb
MPI_STATIC_LIB:= mpi_venc mpi_md
endif

ifeq ($(DEBUG), y)
	PLAT_STATIC_LIB += dmallocthcxx dmallocth
endif

LIBS := $(addprefix $(PLAT_LIB_DIR)/,$(patsubst %,lib%.a,$(PLAT_STATIC_LIB)))
LIBS += $(addprefix $(MPI_LIB_DIR)/,$(patsubst %,lib%.a,$(MPI_STATIC_LIB)))

LDFLAGS := $(LOCAL_LDFLAGS)
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(LOCAL_SHARED_LIB)))
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(MPI_SHARED_LIB)))
LDFLAGS += $(STD_LDFLAGS)

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
TARGET := ak_md_demo

all:$(TARGET)

$(TARGET):$(OBJS)
	$(LD) $^ $(LDFLAGS) -Xlinker "-(" $(LIBS) -Xlinker "-)" -o $@
	$(STRIP) $@

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@
	@echo ""

install:
	$(CP) $(TARGET) $(ROOTFS_BIN_DIR)
	@echo ""

clean:
	$(RM) *.o
	$(RM) $(TARGET)

.PHONY: all clean install
