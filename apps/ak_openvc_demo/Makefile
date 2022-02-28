LIB_SEARCH_PATHS := \
	$(PLAT_WORKDIR)/libplat/lib \
	${OPENCV_PATH}/lib \
	$(CAMLIBS_PATH)/mvs/lib
	
INC_DIRS := \
    $(CUR_PATH) \
	$(PLAT_WORKDIR)/libplat/include \
	${OPENCV_PATH}/include

LIBS := opencv_core \
		opencv_imgproc \
		opencv_highgui

BIN = $(TARGET_BIN)		
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS += $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS += -fno-strict-aliasing -Os -w -D_GNU_SOURCE  -std=c99 -fms-extensions
CXXFLAGS += -fno-strict-aliasing -Os -std=c++0x

CPPFLAGS ?= $(INC_FLAGS) 


space := $(subst ,, )

LDFLAGS := $(LDFLAGS) \
    $(patsubst %,-L%,$(LIB_SEARCH_PATHS)) \
    -Wl,-rpath-link=$(subst $(space),:,$(strip $(LIB_SEARCH_PATHS))) \
    -Wl,-Bstatic \
    -Wl,--start-group $(foreach n, $(STATIC_LIBS), -l$(patsubst lib%,%,$(patsubst %.a,%,$(notdir $(n))))) -Wl,--end-group \
    -Wl,-Bdynamic \
    $(foreach y, $(LIBS), -l$(patsubst lib%,%,$(patsubst %.so,%,$(notdir $(y)))))
    



$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p