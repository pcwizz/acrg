# Arbitrary Cipher Ring Generator
## Generate SVGs for cipher decoding rings for arbitrary alphabets

### Use

Pass it your alphabet via stdin and get an svg out. You may like to use sed to change font size and the like. Your svg can then be rastered by your chosen svg rendering utility, opened in an svg viewer (like icecat or any other modern graphical web browser), or simply admired in text form (if you have an efection for xml). 
`echo "abcdefghijklmnopqrstuvwxyz" | path/to/acrg > mydecoderrings.svg`


### Building

If you would just like to get going:

`./configure.sh; make`

If you are developing:
`./configure.sh debug`

If you ar efeeling paranoid before you commit or make a pull request you can run afl on it:
`./fuzz.sh'

### Defaults

While the program has no real settings (at the moment), as I'm too lazy to do finish it, you may wish to know the defaults to save time when writing your sed script.

- font
	- monospace
- font size
	- 25px
- stroke width
	- 1
- stroke
	- black
- circle fill 
	- none
- outer circle diamiter
	- 400px
- inner circle diamiter
	- 200px

### Things you might run into

The program expects Unicode UTF-8 input. For just about every use I see possible this is fine if you need to use UTF-16, UTF-32 or some strange character encoding, I'm afraid you will have to touch the code, athough try it first it might just work.

### Licence

This project is licenced under the (GPLv3)[LICENCE] as this is an educational tool that produce educational tools I highly support forks and contributions and will accept pull requests as long as they aren't completly obsurd.
