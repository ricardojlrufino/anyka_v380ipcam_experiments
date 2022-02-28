TARGET:= $(wildcard *.so*)

all:
	@echo ""

install:
ifeq ($(TARGET),)
	@echo ""
else
	@$(CP) -d $(TARGET) $(ROOTFS_LIB_DIR)
endif

image:
	@echo ""

clean:
	@echo ""

.PHONY: all install image clean	
