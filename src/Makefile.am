## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = projetf2

projetf2_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h\
	services.c services.h \
	agent.c agent.h \
	gererParking.c gererParking.h \
	agentParking1.c agentParking1.h \
	citoyen.c citoyen.h

projetf2_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

