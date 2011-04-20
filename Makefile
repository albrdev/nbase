CC			:= gcc
CC_FLAGS		:= -Wall -pedantic -ansi
CC_EXT_FLAGS		:= -D_POSIX_C_SOURCE=199309L
CC_LIBS			:= -lm

DIR_SRC			:= ./src
DIR_OBJ			:= ./obj
DIR_BIN			:= ./bin

DIR_INSTALL_BIN		:= /usr/bin

OBJ_ALL_NBASE		:= $(DIR_OBJ)/nbase_main.o $(DIR_OBJ)/nbase.o
OBJ_ALL_STOB		:= $(DIR_OBJ)/stob_main.o $(DIR_OBJ)/nbase.o
OBJ_ALL_BTOS		:= $(DIR_OBJ)/btos_main.o $(DIR_OBJ)/nbase.o

TRG_ALL_NBASE		:= nbase.o nbase_main.o
TRG_ALL_STOB		:= nbase.o stob_main.o
TRG_ALL_BTOS		:= nbase.o btos_main.o

BIN_FILE_NBASE		:= nbase
BIN_FILE_STOB		:= nbase-stob
BIN_FILE_BTOS		:= nbase-btos

PRINT			:= @echo
COPY			:= @cp -v
REMOVE			:= @rm -fv

all: nbase

nbase: $(TRG_ALL_STOB) $(TRG_ALL_BTOS) $(TRG_ALL_NBASE)
	$(CC) $(CC_FLAGS) $(CC_LIBS) $(OBJ_ALL_STOB) -o $(DIR_BIN)/$(BIN_FILE_STOB)
	$(CC) $(CC_FLAGS) $(CC_LIBS) $(OBJ_ALL_BTOS) -o $(DIR_BIN)/$(BIN_FILE_BTOS)
	$(CC) $(CC_FLAGS) $(CC_LIBS) $(OBJ_ALL_NBASE) -o $(DIR_BIN)/$(BIN_FILE_NBASE)
	$(PRINT) "Done!"

stob: $(TRG_ALL_STOB)
	$(CC) $(CC_FLAGS) $(CC_LIBS) $(OBJ_ALL_STOB) -o $(DIR_BIN)/$(BIN_FILE_STOB)
	$(PRINT) "Done!"

btos: $(TRG_ALL_BTOS)
	$(CC) $(CC_FLAGS) $(CC_LIBS) $(OBJ_ALL_BTOS) -o $(DIR_BIN)/$(BIN_FILE_BTOS)
	$(PRINT) "Done!"

stob_main.o: $(DIR_SRC)/stob_main.c
	$(CC) $(CC_FLAGS) $(CC_EXT_FLAGS) -c $? -o $(DIR_OBJ)/$@

btos_main.o: $(DIR_SRC)/btos_main.c
	$(CC) $(CC_FLAGS) $(CC_EXT_FLAGS) -c $? -o $(DIR_OBJ)/$@

nbase_main.o: $(DIR_SRC)/nbase_main.c
	$(CC) $(CC_FLAGS) $(CC_EXT_FLAGS) -c $? -o $(DIR_OBJ)/$@

nbase.o: $(DIR_SRC)/nbase.c
	$(CC) $(CC_FLAGS) -c $? -o $(DIR_OBJ)/$@

clean:
	$(REMOVE) $(DIR_BIN)/$(BIN_FILE_NBASE)
	$(REMOVE) $(DIR_BIN)/$(BIN_FILE_STOB)
	$(REMOVE) $(DIR_BIN)/$(BIN_FILE_BTOS)
	$(REMOVE) $(DIR_OBJ)/*.o

install:
	$(COPY) $(DIR_BIN)/$(BIN_FILE_NBASE) $(DIR_INSTALL_BIN)/$(BIN_FILE_NBASE)
	$(COPY) $(DIR_BIN)/$(BIN_FILE_STOB) $(DIR_INSTALL_BIN)/$(BIN_FILE_STOB)
	$(COPY) $(DIR_BIN)/$(BIN_FILE_BTOS) $(DIR_INSTALL_BIN)/$(BIN_FILE_BTOS)
	$(PRINT) "Done!"

uninstall:
	$(REMOVE) $(DIR_INSTALL_BIN)/$(BIN_FILE_NBASE)
	$(REMOVE) $(DIR_INSTALL_BIN)/$(BIN_FILE_STOB)
	$(REMOVE) $(DIR_INSTALL_BIN)/$(BIN_FILE_BTOS)
	$(PRINT) "Done!"

