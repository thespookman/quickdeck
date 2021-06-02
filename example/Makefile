CARDS = goblin.png troll.png ogre.png
SCRIPTS = $(CARDS:.png=.qd)
DEPS = $(CARDS:.png=.png.d)

.PHONY: all
all: $(CARDS)

-include $(DEPS)

%.png: %.qd make_monsters.qd
	../quickdeck $< make_monsters.qd

$(SCRIPTS): stats.csv
	../params stats.csv

.PHONY: clean
clean:
	rm $(CARDS) $(SCRIPTS) $(DEPS)
