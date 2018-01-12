	APP = realtime
	SOURCES=${APP}.c

	# Opmerking standaard ga ik er van uit dat xeno-config in de /usr/xenomai/bin staat.
	# als dit niet zo is kan je deze makefile oproepen met "make XENODIR=/path/naar/xeno-config"
	XENODIR ?= /usr/xenomai

	# dan staat xeno-config hier:
	XENOCONFIG=$(shell PATH=$(XENODIR):$(XENODIR)/bin:$(PATH) which xeno-config 2>/dev/null)

	#defines voor opmaak:
	RED=\033[0;31m
	YEL=\033[1;33m
	NC=\033[0m 

	# even nakijken:
	ifeq ($(XENOCONFIG),)
		@echo "${RED}!!!! Sorry ik heb xeno-config niet gevonden! roep mij zo op: \"make XENODIR=/path/to/xeno-config\" !!!!${NC}\n"
		@false #Stoppen met builden
	endif

	#compiler + flags instellen: (alchemy in Xenomai3 == native in Xenomai2 )
	CC=$(shell $(XENOCONFIG) --cc)
	CFLAGS=$(shell $(XENOCONFIG) --skin alchemy --cflags) 
	LDFLAGS=$(shell $(XENOCONFIG) --skin alchemy --ldflags) 

	# onderstaande (extra) linker flags voegen de library path toe aan de executable
	# zodat het niet nodig is om het xenomai lib path toe te voegen aan het zoek path
	# maw zonder deze lijn krijg je volgende error bij het runnen v/d applicatie
	# error while loading shared libraries: libtrank.so.0: cannot open shared object file: No such file or directory
	# tenzij er ooit volgende lijn is uitgevoerd in de terminal: export LD_LIBRARY_PATH=/usr/xenomai/lib
	# bekijk https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html voor te weten waarom er 2 keer -Xlinker staat
	LDFLAGS+=-Xlinker -rpath -Xlinker $(shell $(XENOCONFIG) --libdir)


	run: $(APP)
		@echo "${YEL}Running the App (with sudo):${NC}" 
		sudo ./$(APP)

	clean::
		$(RM) $(APP) *.o

	$(APP): $(SOURCES)
		@echo "${YEL}Compiling $< :${NC}" 
		$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@