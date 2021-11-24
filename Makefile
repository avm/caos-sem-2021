guess: guess.S
	gcc -m32 guess.S simpleio_i686.S -o guess

collatz: collatz.S
	gcc -m32 collatz.S simpleio_i686.S -o collatz

%: %.S
	gcc -m32 -g $< simpleio_i686.S -o $@
