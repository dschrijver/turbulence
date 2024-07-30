CC = clang
CC_WIN = x86_64-w64-mingw32-gcc

turbulence: clean_turbulence turbulence.out
	@./turbulence.out
	
turbulence_windows: clean_turbulence_windows turbulence.exe
	@wine ./turbulence.exe

turbulence.out:
	@$(CC) turbulence.c -o turbulence.out -O3 -Wall -Wextra -lraylib -lm 
	
turbulence.exe:
	@$(CC_WIN) turbulence.c -o turbulence.exe -O3 -I ./raylib/src -L ./raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

clean_turbulence:
	@rm -f turbulence.out
	
clean_turbulence_windows:
	@rm -f turbulence.exe
