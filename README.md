Needs more stuff in here, main thing is, if you want to build this, you'll need ImageMagick. Don't know why, but I couldn't get it to work with the packages from the Debian repos, so go to the github repo linked from the ImageMagick website. I compiled ImageMagick without shared libraries so it would automatically link to the static ones, but you do you.

`make build` builds the program.  
`make debug` builds a debug version of the program that sharts loads of info onto standard out.  
`make all` produces both.
