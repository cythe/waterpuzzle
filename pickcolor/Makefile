VERSION = 1.0.0

obj-y += pickcolor

INC_FLAGS += -I $(inc_dir)

INC_FLAGS += $(shell pkgconf --cflags sdl2)
INC_FLAGS += $(shell pkgconf --cflags SDL2_image)
SYS_LIBS += $(shell pkgconf --libs sdl2)
SYS_LIBS += $(shell pkgconf --libs SDL2_image)

EXTRA_LIBDIR += -L $(lib_dir)
EXTRA_LIBS +=

TARGET = $(obj-y)

TARGET_IS_LIB = 0

include $(topdir)/rules.mk
