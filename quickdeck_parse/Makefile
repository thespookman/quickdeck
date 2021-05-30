.PHONY: parser
parser: ../script.yy.c ../script.tab.cc

../script.yy.c: script.l ../script.tab.hh
	flex -o $@ $<

../script.tab.hh: ../script.tab.cc
../script.tab.cc: script.yy
	bison -d $<
	mv script.tab.cc script.tab.hh ..

