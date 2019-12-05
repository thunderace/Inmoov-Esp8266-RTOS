#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := head
EXTRA_COMPONENT_DIRS := $(IDF_LIBS)/components
include $(IDF_LIBS)/make/project.mk
include $(IDF_PATH)/make/project.mk

