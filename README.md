# Introduction

Quickdeck is a program for building decks of cards using a script. I had used nanDECK (http://www.nandeck.com/) in the past, but for my current project I'm using a lot of Linux tools, and didn't want to have to split my workflow between Windows and Linux. That said, Quickdeck _should_ work OK on Windows, assuming ImageMagick is installed, but I haven't tried.

As you'll probably gather, I've just written this to do what I need it to do for one project, with my own needs in mind (e.g. this program is run from the command line, and has no built in editor). I'll probably add more features as I need them, but feel free to open an issue if you'd like me to add anything (I have no intention of adding a GUI or editor, but will happily extend the script parser or add more options for drawing graphics).

# Building

If you want to build this, you'll need ImageMagick. Don't know why (didn't bother to look into it), but I couldn't get it to work with the packages from the Debian repos, so I installed from the github repo linked here: https://www.imagemagick.org/Magick++/. I compiled ImageMagick without shared libraries so it would automatically link to the static ones, because I wanted it statically linked. If you have the shared libraries for Magick++ it _should_ automatically link dynamically.

- `make build` builds both the programs.  
- `make debug` builds a debug version of both programs that sharts loads of info onto standard out.  
- `make all` produces debug and standard versions of both programs.

# Quickdeck Scripting

Usage `quickdeck script1.qd [script2.qd ...]`. Any scripts are run in the given order.

Quickdeck uses a simple scripting language. The syntax is largely C like, but very much not as flexible.

- Scripts consist of statements ended with semicolons (`;`). 
- There are no loops. 
- You can't define functions. 
- Strings must be enclosed in double quotes (`"`). 
- Whitespace and new lines are ignored, you can lay it out however you like.
- Any group of statements can be enclosed in braces (`{}`).
- Reserved keywords are `if` `else` `true` `false`. These are case-insensitive.
- `if condition statement`, `if (condition) statement`, `if (condition) { statements }`, `if condition statement else statement`, `if (condition) statment else { statements }` are all valid ways of laying out if/elses (and the if elses are case-insensitive). Pretty much any way you write it will work.
- All numbers are stored as doubles.
- Valid operators are `+`, `-`, `*`, `/`, `!`, `==`, `!=`, `>`, `<`, `>=`, `<=`, `||`, `&&`. These all have the usual meaning.
- `//` denotes comments.
- Variables are not explicitely typed, `var = 1;` assigns a double value `1.00000` to variable `var`, `var = TRUE;` assigns a boolean true etc.
- Variables are evaluated based on context.
- Numeric variables evaluated as a boolean evaluate as true unless they are 0.
- Numeric variables evaluated as a string evaluate as a string containing the number.
- Boolean variables evaluated as a number evaluate as 1 or 0 for TRUE and FALSE.
- Boolean variables evaluated as a string evaluate as "TRUE" or "FALSE".
- String variables evaluated as a number evaluate as 0.
- String variables evaluated as a boolean evaluate as FALSE.
- `echo`, `new`, `save`, `rect`, `text`, `image` and `line` are reserved as function names.
- After a function name, parameters should be added in brackets (`()`), separated by commas (`,`).
- Colours are represented as standard strings, like `#000000` or `#00000000` including opacity.

## Function definitions

### echo ( var1, var2 ... )

Prints all parameters to stdout.

### new ( width, height )

Any drawing functions called before new will be ignored. This creates a blank canvas of the size specified in the parameters. Any drawing functions called will be drawn to this canvas. Calling new when a canvas already exists will overwrite the current canvas.

### save ( filename )

Saves the canvas to the file specified. File type is inferred from extension. Also creates a file with the filename+`.d` containing Makefile dependencies for any image files that were read while the script was running.

### rect ( fill, x, y, width, height, thickness, colour)

Draws a rectangle at (x, y) of size width x height. If fill is true, it will be filled, otherwise just an outline. Thickness is the thickness of the lines.

### text ( text, font, pointsize, x, y, colour)

Draws text at (x, y). Font the path of a font file. If font is left as `""` you'll get the default system font.

### image ( filename, x, y, width, height )

Draws the image found at filename at (x, y). It will be scaled to width x height. Also adds the image as a dependency (see `save`).

### line ( x1, y1, x2, y2, thickness, colour )

Draws a line from (x1, y1) to (x2, y2), thickness pixels wide.

# Params

Params is a utility for turning a csv file into a Quickdeck script of variable assignments. The header row of the csv should contain the variable names, subsequent rows contain the data. One file is produced per row. Files are only written if they would be changed. This allows the user to define data for the cards in a single file, which can then be built using quickdeck and GNU make.

# Using them together

_All of the files for this example are included in the repo in `example/`. If you want to follow along, run `make build` at the base of the repo, cd to the example folder and do a `make clean`._

Say we have a csv `stats.csv`

```
title, backgroundColour, power                                                                                                      
"goblin", "#00FF00", "5"
"troll", "#FF0000", "10"
"ogre", "#0000FF", "15"
```

and we run `params stats.csv`, we get three files:  
`goblin.qd` - `title = "goblin"; backgroundColour = "#00FF00"; power = "5";`  
`troll.qd` - `title = "troll"; backgroundColour = "#FF0000"; power = "10";`  
`ogre.qd` - `title = "ogre"; backgroundColour = "#0000FF"; power = "15";`  

We can write a script `make_monsters.qd`:

```
new(200,400);
image("resources/background.png", 0, 0, 200, 400);
rect(true, 0, 0, 200, 400, 1, backgroundColour+"88");
rect(false, 5, 5, 190, 390, 1, "#000000");
text(title, "", 10, 10, 20, "#000000");
image("resources/"+title+".png", 10, 50, 180, 180);
text("Power: "+power, "", 20, 10, 300, "#000000");
save(title + ".png");
```

And a Makefile:

```
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
```

Now `make all` produces the following images:

![Goblin](https://raw.githubusercontent.com/thespookman/quickdeck/main/example/goblin.png?token=ACJ7E5AJGR42R35KGOFJPLDAWTQLW)
![Troll](https://raw.githubusercontent.com/thespookman/quickdeck/main/example/troll.png?token=ACJ7E5BEMKRPPPF4TN2K5DLAWTQOA)
![Ogre](https://raw.githubusercontent.com/thespookman/quickdeck/main/example/ogre.png?token=ACJ7E5HQYB6FXJZKGHJCQNLAWTQMW)

Also, any changes to any of the images, the data in the csv, or `make_monsters.qd` will only cause the necessary images to be rebuilt!
