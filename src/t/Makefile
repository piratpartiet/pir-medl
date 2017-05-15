# Makefile
# File ID: 23b8f6b4-38e8-11e7-97d5-f74d993421b0

.PHONY: test
test: build
	./medl-add.perl.t
	./medl-add.t
	./medl-init.t

.PHONY: build
build:
	cd .. && $(MAKE)

.PHONY: clean
clean:
