all:
	cd src && $(MAKE)
	cp src/nflog2eth ./
clean: 
	rm -f nflog2eth
