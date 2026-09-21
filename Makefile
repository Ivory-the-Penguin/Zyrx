CC = clang
CFLAGS = -std=c99 -g -Isrc -Wall -Wno-unused-function -Werror

zyrx: src/main.c compile_flags.txt
	@mkdir -p build
	$(CC) $(CFLAGS) src/main.c -o build/zyrx

compile_flags.txt: Makefile
	@echo "Generating compile_flags.txt..."
	@echo "$(CFLAGS)" | tr ' ' '\n' > compile_flags.txt

run: zyrx
	@echo
	@./build/zyrx

clean:
	@rm -rf build
