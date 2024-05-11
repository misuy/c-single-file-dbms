.PHONY: all clean build run


CC_FLAGS = -Werror -Wall -Wmissing-prototypes -Wstrict-prototypes $(shell pkg-config --cflags gobject-2.0)
CC = gcc

LD_FLAGS = -lglib-2.0 -lgobject-2.0 -lthrift_c_glib -lm -lc


BUILD_DIR = build
SRC_DIR = src


CLIENT_BUILD_DIR = $(BUILD_DIR)/client
CLIENT_TARGET = $(BUILD_DIR)/lab3-client

CLIENT_SRC_DIR = $(SRC_DIR)/client
CLIENT_TO_BUILD = \
	$(CLIENT_BUILD_DIR)/finite_automata.o \
	$(CLIENT_BUILD_DIR)/lex.o \
	$(CLIENT_BUILD_DIR)/syntax.o \
	$(CLIENT_BUILD_DIR)/text.o \
	$(CLIENT_BUILD_DIR)/token.o \
	$(CLIENT_BUILD_DIR)/tree.o \
	$(CLIENT_BUILD_DIR)/parser.o \
	$(CLIENT_BUILD_DIR)/main.o \


SERVER_BUILD_DIR = $(BUILD_DIR)/server
SERVER_TARGET = $(BUILD_DIR)/lab3-server

SERVER_SRC_DIR = $(SRC_DIR)/server
SERVER_TO_BUILD = \
	$(SERVER_BUILD_DIR)/file.o \
	$(SERVER_BUILD_DIR)/table.o \
	$(SERVER_BUILD_DIR)/cell.o \
	$(SERVER_BUILD_DIR)/raw_slot.o \
	$(SERVER_BUILD_DIR)/slot.o \
	$(SERVER_BUILD_DIR)/page.o \
	$(SERVER_BUILD_DIR)/predicate.o \
	$(SERVER_BUILD_DIR)/row.o \
	$(SERVER_BUILD_DIR)/interface.o \
	$(SERVER_BUILD_DIR)/query.o \
	$(SERVER_BUILD_DIR)/transformation.o \
	$(SERVER_BUILD_DIR)/data_iterator.o \
	$(SERVER_BUILD_DIR)/handler.o \
	$(SERVER_BUILD_DIR)/main.o


PROTOCOL_BUILD_DIR = $(BUILD_DIR)/protocol

PROTOCOL_SRC_DIR = $(SRC_DIR)/protocol
PROTOCOL_TO_BUILD = \
	$(PROTOCOL_BUILD_DIR)/protocol_types.o \
	$(PROTOCOL_BUILD_DIR)/query_executor.o \
	$(PROTOCOL_BUILD_DIR)/util.o

$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(CLIENT_BUILD_DIR))
$(shell mkdir -p $(SERVER_BUILD_DIR))
$(shell mkdir -p $(PROTOCOL_BUILD_DIR))


all: build


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $^


$(CLIENT_TARGET): $(CLIENT_TO_BUILD) $(PROTOCOL_TO_BUILD)
	$(CC) $(LD_FLAGS) -o $@ $^

build-client: $(CLIENT_TARGET)


$(SERVER_TARGET): $(SERVER_TO_BUILD) $(PROTOCOL_TO_BUILD)
	$(CC) $(LD_FLAGS) -o $@ $^

build-server: $(SERVER_TARGET)


build: build-client build-server


clean:
	rm -rf $(BUILD_DIR)