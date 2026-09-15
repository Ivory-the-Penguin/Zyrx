CC = clang
CFLAGS = -std=c99 -Wall -Wextra -g

zyrx: src/main.c 
	@mkdir -p build
	$(CC) $(CFLAGS) src/main.c -o build/zyrx

run: zyrx
	@echo
	@./build/zyrx

clean:
	@rm -rf build