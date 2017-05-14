# pir-medl.git/Makefile
# File ID: 8eaa0266-38c4-11e7-8a1b-f74d993421b0

.PHONY: default
default:

.PHONY: clean
clean:
	cd t && $(MAKE) clean

.PHONY: test
test:
	cd t && $(MAKE) test
