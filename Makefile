# Macros para compilacao
CC = gcc
CFLAGS = -Wextra -lpthread -I./ -lfreeimage
DIR = src

# Macros para arquivos fonte
ILSRC = $(DIR)/inline.c
MPSRC = $(DIR)/multiprocess.c
MTSRC = $(DIR)/multithread.c

# Macros para as bibliotecas
LIBS = lib/*.c

# Macros para arquivos  de saída
ILOUT = ./inline
MPOUT = ./multiprocess
MTOUT = ./multithread
IMG_DIR = ./images
DOCS_DIR = ./docs
TIME_FILE = time-report.csv

# Macros para teste
BASH = sh
SCRIPT = test.sh

# Macros para construcao do zip
ZIP = zip
USERNAME ?= 116528-170272-174638
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.sh *.pdf

# Header do arquivo de medição de tempo
TIME_TITLE = "Method, Number of Process/Threads, Height, Width, Real Time, User Time\n"

all:inline multiprocess multithread

inline:
	$(CC) -o$(ILOUT) $(ILSRC) $(LIBS) $(CFLAGS)

multiprocess:
	$(CC) -o$(MPOUT) $(MPSRC) $(LIBS) $(CFLAGS)

multithread:
	$(CC) -o$(MTOUT) $(MTSRC) $(LIBS) $(CFLAGS)

zip:clean
	$(ZIP) -R $(ZIPFILE) Makefile $(EXTENSIONS)

clean:
	$(RM) $(ILOUT)
	$(RM) $(MPOUT)
	$(RM) $(MTOUT)
	$(RM) ./$(ZIPFILE)
	$(RM) $(IMG_DIR)/*filtered*
	$(RM) $(DOCS_DIR)/$(TIME_FILE)

test:clean all
	echo $(TIME_TITLE) > $(DOCS_DIR)/$(TIME_FILE)
	$(BASH) $(SCRIPT) $(ILOUT) $(IMG_DIR)
	$(BASH) $(SCRIPT) $(MPOUT) $(IMG_DIR)
	$(BASH) $(SCRIPT) $(MTOUT) $(IMG_DIR)

