CC=clang
LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
CFLAGS=-Wall \
	-Wextra \
	-Wconversion \
	-Wshorten-64-to-32 \
	-Wconstant-conversion \
	-Wliteral-range \
	-Woverflow \
	-Wimplicit-int \
	-Wreserved-identifier \
	-pedantic-errors \
	-std=c23 \
	-fsanitize=address,undefined \
	-g \
	-O0


ifndef VERBOSE
MAKEFLAGS += --no-print-directory
endif

BIN_DIR=bin

# Deduplicating is necessary for the case where we're compiling the utils projects, to avoid including the util lib.c file twice which would lead to duplicate symbol errors.
SOURCES = $(shell echo ./*.c ../utils/*/lib.c | xargs -n 1 realpath 2>/dev/null | sort | uniq)

$(BIN_DIR)/test: $(SOURCES)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $^

test: $(BIN_DIR)/test
# ASan allocates memory before malloc which causes the macOS malloc to complain about being unable
# to get its "nano zone"
	@MallocNanoZone=0 $<
