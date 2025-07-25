#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=avr-gcc
CCC=avr-g++
CXX=avr-g++
FC=gfortran
AS=avr-as

# Macros
CND_PLATFORM=AVR-Linux
CND_DLIB_EXT=so
CND_CONF=Custom
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include /home/dode/dev/avr64ea/Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/ce5ff35b/avr64ea.o \
	${OBJECTDIR}/_ext/ce5ff35b/rfm.o \
	${OBJECTDIR}/_ext/ce5ff35b/spi.o \
	${OBJECTDIR}/_ext/ce5ff35b/usart.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avr64ea

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avr64ea: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/avr64ea ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/ce5ff35b/avr64ea.o: /home/dode/dev/avr64ea/avr64ea.c
	${MKDIR} -p ${OBJECTDIR}/_ext/ce5ff35b
	$(COMPILE.c) -g -DBAUD=9600 -DBAUDRATE=9600 -DF_CPU=10000000UL -DRFM=69 -D__AVR_AVR64EA28__ -D__flash=volatile -I. -I/home/dode/dev -std=c99 -o ${OBJECTDIR}/_ext/ce5ff35b/avr64ea.o /home/dode/dev/avr64ea/avr64ea.c

${OBJECTDIR}/_ext/ce5ff35b/rfm.o: /home/dode/dev/avr64ea/rfm.c
	${MKDIR} -p ${OBJECTDIR}/_ext/ce5ff35b
	$(COMPILE.c) -g -DBAUD=9600 -DBAUDRATE=9600 -DF_CPU=10000000UL -DRFM=69 -D__AVR_AVR64EA28__ -D__flash=volatile -I. -I/home/dode/dev -o ${OBJECTDIR}/_ext/ce5ff35b/rfm.o /home/dode/dev/avr64ea/rfm.c

${OBJECTDIR}/_ext/ce5ff35b/spi.o: /home/dode/dev/avr64ea/spi.c
	${MKDIR} -p ${OBJECTDIR}/_ext/ce5ff35b
	$(COMPILE.c) -g -DBAUD=9600 -DBAUDRATE=9600 -DF_CPU=10000000UL -DRFM=69 -D__AVR_AVR64EA28__ -D__flash=volatile -I. -I/home/dode/dev -o ${OBJECTDIR}/_ext/ce5ff35b/spi.o /home/dode/dev/avr64ea/spi.c

${OBJECTDIR}/_ext/ce5ff35b/usart.o: /home/dode/dev/avr64ea/usart.c
	${MKDIR} -p ${OBJECTDIR}/_ext/ce5ff35b
	$(COMPILE.c) -g -DBAUD=9600 -DBAUDRATE=9600 -DF_CPU=10000000UL -DRFM=69 -D__AVR_AVR64EA28__ -D__flash=volatile -I. -I/home/dode/dev -o ${OBJECTDIR}/_ext/ce5ff35b/usart.o /home/dode/dev/avr64ea/usart.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
