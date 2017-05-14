# pir-medl.git/Makefile
# File ID: 8eaa0266-38c4-11e7-8a1b-f74d993421b0

.PHONY: default
default: README.html
	cd src && $(MAKE)

README.html: README.md
	printf '<html>\n<head>\n' >README.html
	printf '<meta http-equiv="Content-Type" ' >>README.html
	printf 'content="text/html; charset=UTF-8" />\n' >>README.html
	printf '<title>pir-medl/README</title>\n' >>README.html
	printf '</head>\n<body>\n' >>README.html
	cmark -t html README.md >>README.html
	printf '</body>\n</html>\n' >>README.html

.PHONY: clean
clean:
	rm -f README.html
	cd src && $(MAKE) clean
	cd t && $(MAKE) clean

.PHONY: edit
edit:
	$(EDITOR) $$(git ls-files | grep -v -e ^COPYING)

.PHONY: test
test:
	cd src && $(MAKE) test
	cd t && $(MAKE) test

.PHONY: view
view: README.html
	lynx README.html
