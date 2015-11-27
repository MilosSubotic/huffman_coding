#
###############################################################################

clean:
	-rm -rf work/*
	-mv docs/tex/*.pdf docs/
	-$(foreach f, *.aux *.log *.out *.synctex.gz, rm -f docs/tex/${f};)

distclean: clean

###############################################################################

dist: distclean
	cd ../ && zip -9r \
		huffman_coding-$$(date +%F-%T | sed 's/:/-/g').zip \
		huffman_coding

###############################################################################
