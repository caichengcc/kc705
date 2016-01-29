COMPILER=
ARCHIVER=
CP=cp
COMPILER_FLAGS=
EXTRA_COMPILER_FLAGS=
FLIP_REG_ENDIAN_COMPILER_FLAG=
LIB=libdrivers.a

RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR}

LIBSOURCES=src/*.c osdrv/*.c
OUTS = *.o

include makefile.include


libs:
	echo "Compiling logiCVC"
	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(TCL_GEN_FLAGS) $(INCLUDES) $(LIBSOURCES)
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OUTS}
	make clean

include:


clean:
	rm -rf ${OUTS}
