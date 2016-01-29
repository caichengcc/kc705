COMPILER=
ARCHIVER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=

# MAIN switch for the oslib type
OSLIB=possix
OSLIBBUSTYPE=cpu
# MAIN switch for the oslib type

LIB=libdrivers.a
RELEASEDIR=../../../lib
ROOTINCLUDEDIR=../../../include


INCLUDEDIR=../../../include/xyl_oslib

INCLUDES=-I${ROOTINCLUDEDIR} \
			-I${INCLUDEDIR} \
			-I${INCLUDEDIR}/common/inc/ \
			-I${INCLUDEDIR}/${OSLIB}/common/inc/ \
			-I${INCLUDEDIR}/${OSLIB}/common/src/ \
			-I${INCLUDEDIR}/${OSLIB}/cpu/inc/ \
			-I${INCLUDEDIR}/${OSLIB}/cpuVlink/inc

ISF_DIR = .

#			$(ISF_DIR)/${OSLIB}/${OSLIBBUSTYPE}/src/OsHwCpuAccessConfig.c

LIB_SRCS = $(ISF_DIR)/common/src/OslibHwAccess.c \
			$(ISF_DIR)/common/src/OslibMemory.c \
			$(ISF_DIR)/${OSLIB}/common/src/OsMisc.c
OUTS = *.o

# create ISF_SRCS based on configured options

ISF_SRCS = $(LIB_SRCS)

ISF_OBJS = $(ISF_SRCS:%.c=%.o)


include makefile.include

libs:
	echo "Compiling Xylon OS Library"
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(TCL_GEN_FLAGS) -DXYL_OSLIB_POSSIX_XILINX_BSP $(INCLUDES) $(LIB_SRCS)
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OUTS}
	make clean

include:

clean:
	rm -rf *.o

xilOSlib.a: obj_dir print_msg_flash_base $(ISF_OBJS)
	@echo "Creating archive $@"
	$(ARCHIVER) rc $@ obj/*.o

obj_dir:
	mkdir -p obj

print_msg_flash_base:
	@echo "Compiling Xylon OS Library:"

.c.o:
	@echo "Compiling $<"
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) -c $< -o obj/$(@F)
