# appointed libs and their dir

COMPILE_SHARED := y
 
# Params from 
CFLAGS := -fno-strict-aliasing -Os
CXXFLAGS := -fno-strict-aliasing -Os
STD_LDFLAGS = -lrt -lpthread -ldl -Wl,-rpath=/mnt -rdynamic
	
CFLAGS += -Wall -Werror -D_GNU_SOURCE -std=c99 -fms-extensions
LD:= $(CC)
LDCC := $(CC)
LDCXX := $(CXX)

# Params from apps

PLAT_LIB_DIR := $(PLAT_WORKDIR)/libplat/lib
PLAT_INC_DIR := $(PLAT_WORKDIR)/libplat/include

PLAT_LIB_DIR += $(PLAT_WORKDIR)/libmpi/lib
PLAT_INC_DIR += $(PLAT_WORKDIR)/libmpi/include
	
CFLAGS += $(addprefix -I,$(PLAT_INC_DIR))
CFLAGS += $(addprefix -I,$(PLAT_INC_DIR_INNER))


LOCAL_SHARED_LIB := akaudiofilter akaudiocodec mpi_aenc plat_ai plat_common plat_thread plat_ipcsrv


LOCAL_LDFLAGS := $(addprefix -L,$(PLAT_LIB_DIR))
LOCAL_LDFLAGS += $(addprefix $(PLAT_LIB_DIR)/,$(patsubst %,lib%.a,$(LOCAL_STATIC_LIB)))

LDFLAGS := $(LOCAL_LDFLAGS)
LDFLAGS += $(addprefix -l,$(patsubst lib%,%,$(LOCAL_SHARED_LIB)))
LDFLAGS += $(STD_LDFLAGS)

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))
TARGET := $(TARGET_BIN)

all:$(TARGET)

$(TARGET): $(OBJS)
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
	
.PHONY: all install clean 
