# Macros
CC					= gcc
CC_FLAGS			= -pedantic -ansi -std=c99 -Wall -Wextra -Wconversion
DBG_FLAGS			= -DNBASE_DEBUG
RLS_FLAGS			= -DNDEBUG
CC_LIBS				= -lm

DIR_SRC				= src
DIR_OBJ				= obj
DIR_BIN				= bin
DIR_INSTALL			= /usr/local/bin

TRG_OBJECTS			= base.o nbase.o args.o stob_main.o btos_main.o

OBJS				= ./$(DIR_OBJ)/base.o ./$(DIR_OBJ)/nbase.o ./$(DIR_OBJ)/args.o

BIN_STOB_RELEASE	= nbase_stob
BIN_STOB_DEBUG		= nbase_stob_d
BIN_BTOS_RELEASE	= nbase_btos
BIN_BTOS_DEBUG		= nbase_btos_d
BIN_USAGE			= [OPTION]... [TARGET]...

NAME				= nBase
VERSION				= 1.2b
AUTHOR				= albrdev
LICENSE				= GNU GPLv3
WEBSITE				= https://github.com/albrdev/nbase

CMD_CP				= /bin/cp -f
CMD_RM				= /bin/rm -f
CMD_PRINT			= @printf
CMD_DATE			= @/bin/date
CMD_UNAME			= @uname -om

# All
all: release

release: pre_print cli_release post_print
debug: pre_print cli_debug post_print

# Print
pre_print:
	$(CMD_PRINT) "[build]\n"

	$(CMD_PRINT) "Compiler:\t$(CC)\n"
	$(CMD_PRINT) "Flags:\t\t$(CC_FLAGS)\n"
	$(CMD_PRINT) "Libraries:\t$(CC_LIBS)\n\n"

post_print:
	$(CMD_PRINT) "[done]\n\n"

	$(CMD_DATE)
	$(CMD_UNAME)
	$(CMD_PRINT) "\n"

	$(CMD_PRINT) "$(NAME) v$(VERSION)\n"
	$(CMD_PRINT) "Author:\t\t$(AUTHOR)\n"
	$(CMD_PRINT) "License:\t$(LICENSE)\n"
	$(CMD_PRINT) "Website:\t$(WEBSITE)\n\n"

# Compilation
cli_release: CC_FLAGS += $(RLS_FLAGS) -DAPPINFO_BINARY="\"$(BIN_RELEASE)\"" -DAPPINFO_USAGE="\"$(BIN_USAGE)\"" -DAPPINFO_NAME="\"$(NAME)\"" -DAPPINFO_VERSION="\"$(VERSION)\"" -DAPPINFO_AUTHOR="\"$(AUTHOR)\"" -DAPPINFO_LICENSE="\"$(LICENSE)\"" -DAPPINFO_WEBSITE="\"$(WEBSITE)\""
cli_release: $(TRG_OBJECTS)
	@$(CC) -x none $(CC_FLAGS) $(OBJS) ./$(DIR_OBJ)/stob_main.o $(CC_LIBS) -o ./$(DIR_BIN)/$(BIN_STOB_RELEASE)
	$(CMD_PRINT) "\n./$(DIR_OBJ)/*.o\t\t-> ./$(DIR_BIN)/$(BIN_STOB_RELEASE)"

	@$(CC) -x none $(CC_FLAGS) $(OBJS) ./$(DIR_OBJ)/btos_main.o $(CC_LIBS) -o ./$(DIR_BIN)/$(BIN_BTOS_RELEASE)
	$(CMD_PRINT) "\n./$(DIR_OBJ)/*.o \t\t-> ./$(DIR_BIN)/$(BIN_BTOS_RELEASE)"

cli_debug: CC_FLAGS += $(DBG_FLAGS) -DAPPINFO_BINARY="\"$(BIN_RELEASE)\"" -DAPPINFO_USAGE="\"$(BIN_USAGE)\"" -DAPPINFO_NAME="\"$(NAME)\"" -DAPPINFO_VERSION="\"$(VERSION)\"" -DAPPINFO_AUTHOR="\"$(AUTHOR)\"" -DAPPINFO_LICENSE="\"$(LICENSE)\"" -DAPPINFO_WEBSITE="\"$(WEBSITE)\""
cli_debug: $(TRG_OBJECTS)
	@$(CC) -x none $(CC_FLAGS) $(OBJS) ./$(DIR_OBJ)/stob_main.o $(CC_LIBS) -o ./$(DIR_BIN)/$(BIN_STOB_DEBUG)
	$(CMD_PRINT) "\n./$(DIR_OBJ)/*.o\t\t-> ./$(DIR_BIN)/$(BIN_STOB_DEBUG)"

	@$(CC) -x none $(CC_FLAGS) $(OBJS) ./$(DIR_OBJ)/btos_main.o $(CC_LIBS) -o ./$(DIR_BIN)/$(BIN_BTOS_DEBUG)
	$(CMD_PRINT) "\n./$(DIR_OBJ)/*.o\t\t-> ./$(DIR_BIN)/$(BIN_BTOS_DEBUG)"

# Objects
base.o: ./$(DIR_SRC)/base.c
	@$(CC) -x c $(CC_FLAGS) -c $? -o ./$(DIR_OBJ)/$@
	$(CMD_PRINT) "./$?\t\t-> ./$(DIR_OBJ)/$@\n"

nbase.o: ./$(DIR_SRC)/nbase.c
	@$(CC) -x c $(CC_FLAGS) -c $? -o ./$(DIR_OBJ)/$@
	$(CMD_PRINT) "./$?\t\t-> ./$(DIR_OBJ)/$@\n"

args.o: ./$(DIR_SRC)/args.c
	@$(CC) -x c $(CC_FLAGS) -c $? -o ./$(DIR_OBJ)/$@
	$(CMD_PRINT) "./$?\t\t-> ./$(DIR_OBJ)/$@\n"

stob_main.o: ./$(DIR_SRC)/stob_main.c
	@$(CC) -x c $(CC_FLAGS) -c $? -o ./$(DIR_OBJ)/$@
	$(CMD_PRINT) "./$?\t-> ./$(DIR_OBJ)/$@\n"

btos_main.o: ./$(DIR_SRC)/btos_main.c
	@$(CC) -x c $(CC_FLAGS) -c $? -o ./$(DIR_OBJ)/$@
	$(CMD_PRINT) "./$?\t-> ./$(DIR_OBJ)/$@\n"

# Install
.PHONY: install
install:
	$(CMD_PRINT) "[install]\n"
	$(CMD_CP) ./$(DIR_BIN)/$(BIN_STOB_RELEASE) $(DIR_INSTALL)/$(BIN_STOB_RELEASE)
	$(CMD_CP) ./$(DIR_BIN)/$(BIN_BTOS_RELEASE) $(DIR_INSTALL)/$(BIN_BTOS_RELEASE)
	$(CMD_PRINT) "[done]\n\n"

# Uninstall
.PHONY: uninstall
uninstall:
	$(CMD_PRINT) "[uninstall]\n"
	$(CMD_RM) $(DIR_INSTALL)/$(BIN_STOB_RELEASE)
	$(CMD_RM) $(DIR_INSTALL)/$(BIN_BTOS_RELEASE)
	$(CMD_PRINT) "[done]\n\n"

# Cleanup
.PHONY: clean
clean:
	$(CMD_PRINT) "[clean]\n"
	$(CMD_RM) ./$(DIR_OBJ)/*.o
	$(CMD_RM) ./$(DIR_BIN)/$(BIN_STOB_RELEASE)
	$(CMD_RM) ./$(DIR_BIN)/$(BIN_BTOS_RELEASE)
	$(CMD_RM) ./$(DIR_BIN)/$(BIN_STOB_DEBUG)
	$(CMD_RM) ./$(DIR_BIN)/$(BIN_BTOS_DEBUG)
	$(CMD_PRINT) "[done]\n\n"
