# Compiler
CC = emcc

# Paths
RAYLIB_SRC = $(CURDIR)/raylib/src
DIST_DIR   = dist

# Files
TARGET = $(DIST_DIR)/index.html
INC_DIR = include
INC    = -I$(INC_DIR)
SRC    = $(wildcard src/*.c)

# Cross-platform mkdir -p
ifeq ($(OS),Windows_NT)
    MKDIR_P = powershell -Command "if (-not (Test-Path '$(1)')) { New-Item -ItemType Directory -Path '$(1)' | Out-Null }"
else
    MKDIR_P = mkdir -p $(1)
endif

# Flags
CFLAGS  = -Wall -I$(RAYLIB_SRC)
LDFLAGS = $(RAYLIB_SRC)/libraylib.web.a \
          -s USE_GLFW=3 \
          -s ASYNCIFY \
          -s FORCE_FILESYSTEM=1 \
          --preload-file assets \
          -s INITIAL_MEMORY=67108864 \
          -s MAX_WEBGL_VERSION=2 \
          --shell-file shell.html \
          -s ASSERTIONS=2 \
          -g4 \
          --source-map-base http://127.0.0.1:5500/dist/ \
          -s WASM=1 \
          -s STACK_OVERFLOW_CHECK=2 \
          -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(SRC)
	@$(call MKDIR_P,$(DIST_DIR))
	$(CC) $(SRC) -o $@ $(CFLAGS) $(INC) $(LDFLAGS)

# Clean rules
clean:
	@echo [CLEAN]
	@rm -rf $(DIST_DIR)

.PHONY: all clean
