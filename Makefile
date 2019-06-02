# Macros para compilacao
CC = gcc
CFLAGS = -Wextra -lpthread -I./ -lfreeimage
DIR = src
BJS = $(SRCS:.c=.o)

# Macros para arquivos fonte
ILSRC = $(DIR)/inline.c
MPSRC = $(DIR)/multiprocess.c
MTSRC = $(DIR)/multithread.c

# Macros para as bibliotecas
LIBS = lib/*.c

# Macros para arquivos  de saída
ILOUT = inline
MPOUT = multiprocess
MTOUT = multithread

# Macros para teste
BASH = sh
TEST_SCRIPT = test.sh
VERBOSE ?= 1

# Macros para construcao do zip
ZIP = zip
USERNAME ?= 170272-xxxxxx-xxxxxx
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.sh *.pdf

inline:
	$(CC) -o$(ILOUT) $(ILSRC) $(LIBS) $(CFLAGS)

multiprocess:
	$(CC) -o$(MPOUT) $(MPSRC) $(CFLAGS)

multithread:
	$(CC) -o$(MTOUT) $(MTSRC) $(CFLAGS)

zip:clean
	$(ZIP) -R $(ZIPFILE) Makefile $(EXTENSIONS)

clean:
	$(RM) ./$(ILOUT)
	$(RM) ./$(MPOUT)
	$(RM) ./$(MTOUT)
	$(RM) $(DIR)/*.o
	$(RM) ./$(ZIPFILE)
