CUR_DIR:=$(shell pwd)
LOCAL_LDFLAGS := -L$(PLAT_LIB_DIR) -L$(MPI_LIB_DIR)
LOCAL_INC_DIR := $(CUR_DIR)/include $(PLAT_INC_DIR_INNER)
CFLAGS += $(addprefix -I,$(LOCAL_INC_DIR))

export LOCAL_LDFLAGS

dir_y := aenc_demo
dir_y += adec_demo
dir_y += venc_demo
dir_y += md_demo
dir_y += osd_demo
dir_y += mt_demo

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

.PHONY: all install clean
