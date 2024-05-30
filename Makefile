turbulence: clean_turbulence turbulence.out
	@./turbulence.out

turbulence.out:
	@clang turbulence.c -o turbulence.out -O3 -Wall -Wextra -lraylib -lm 

clean_turbulence:
	@rm -f turbulence.out
