.PHONY: parser
parser: ../data.yy.c ../data.tab.cc

../data.yy.c: data.l ../data.tab.hh
	flex -o $@ $<

../data.tab.hh: ../data.tab.cc
../data.tab.cc: data.yy
	bison -d $<
	mv data.tab.cc data.tab.hh ..

