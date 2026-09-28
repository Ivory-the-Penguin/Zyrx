CC = clang
CFLAGS = -std=gnu11 -g -Wall -Wno-unused-function -Werror \
      	-Isrc \
      	-Ivendor \
      	-MMD -MP

LDFLAGS = -lm 
NAME = zyrx
BUILD_DIR = build

SRCS = $(shell find src -name "*.c") $(shell find vendor -name "*.c")

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

$(NAME): $(OBJS) compile_flags.txt
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

compile_flags.txt: Makefile
	@echo "Generating compile_flags.txt..."
	@echo "$(CFLAGS)" | tr ' ' '\n' > compile_flags.txt

run: $(NAME)
	@echo
	@./$(NAME)

clean:
	rm -rf $(BUILD_DIR) $(NAME) compile_flags.txt

.PHONY: run clean
