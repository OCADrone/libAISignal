# Makefile for the libAIRegistry libray.
# Version: 2
#
# Author       : Jean-Philippe Clipffel
# Creation date: 29/04/2014
# Last modified: 17/10/2014


# Variables
# =========

NAME 						= 		libAISignal.so
LIB_NAME 				= 		libAISignal.so
LIB_VERSION 		= 		2

PATH_ROOT 			= 		.
PATH_SOURCE 		= 		./source/AISignal
PATH_INCLUDE 		= 		./include/AISignal

INCLUDE 				= 		./include

INSTALL_LIB 		= 		/usr/local/lib
INSTALL_INCLUDE = 		/usr/local/include/AISignal
INSTALL_SOURCES =			/usr/src/libAISignal



# Lib sources files
# =================
SRC 					=		$(PATH_SOURCE)/server.cpp 			\
									$(PATH_SOURCE)/client.cpp 			\
									$(PATH_SOURCE)/request.cpp 			\
									$(PATH_SOURCE)/answer.cpp				\
									$(PATH_SOURCE)/channel.cpp



# Compilation options
# ===================
CC 						=		g++
CFLAGS 				=		-W -Wall -ansi -pedantic -O3 -std=c++11
CLIBS 				=


# Rules configuration
# ====================
.PHONY: 	help build clean 										\
			install install-headers install-library install-sources \
			remove remove-headers remove-library remove-sources

.SILENT:	help build clean 										\
			install install-headers install-library install-sources \
			remove remove-headers remove-library remove-sources



# Basic rules
# ===========
$(NAME):	help

help:
			@echo "libAISignal Makefile help"
			@echo "========================="

			@echo "See bellow for generic help. For help about using libAISignal, type 'make help-usage'"
			@echo ""
			@echo "[*] Compilation"
			@echo "    Type 'make build' to build the library. '$(LIB_NAME)' is then created."
			@echo ""
			@echo "[*] Installation"
			@echo "   To install the whole package (except sources): 'make install'"
			@echo "   To install only the headers: 'make install-headers'"
			@echo "   To install only the library: 'make install-library'"
			@echo "   To install the sources (disable by default): 'make install-sources'"
			@echo ""
			@echo "[*] Removing"
			@echo "   To remove the whole installation (except sources): 'make remove'"
			@echo "   To remove only the headers: 'make remove-headers'"
			@echo "   To remove only the library: 'make remove-library'"
			@echo "   To remove the sources (disable by default): 'make remove-sources'"
			@echo ""
			@echo "[*] Other"
			@echo "    You can remove temporary files using 'make clean'"

help-usage:
			@echo "Compile with libAISignal"
			@echo "========================"
			@echo ""
			@echo "You have two options:"
			@echo "1) Add $(INSTALL_LIB) to your LD_LIBRAY_PATH variable:"
			@echo "   'export LD_LIBRARY_PATH=/usr/local/lib:\$$LD_LIBRARY_PATH'"
			@echo "   From now, compile with 'g++ <sources> -lAIRegistry'"
			@echo "OR"
			@echo "2) Compile with:"
			@echo "   'g++ <sources> -lAISignal -Wl,-rpath $(INSTALL_LIB)'"


# Compilation rules
# =================
build:
			@echo "--------------------------------------------"
			@echo "Compiling $(LIB_NAME) version $(LIB_VERSION)"
			@echo "Compiler       : $(CC)"
			@echo "Compiler flags : $(CFLAGS)"
			@echo "Libraires      : $(CLIBS)"
			@echo "--------------------------------------------"
			$(CC) $(CFLAGS) $(CLIBS) -I $(INCLUDE) -fPIC -shared $(SRC) -o $(NAME)


# Installation rules
# ==================
install-headers:
			@echo "[*] Copying AISignal classes headers"
			mkdir -p $(INSTALL_INCLUDE)
			cp -r $(PATH_INCLUDE)/* $(INSTALL_INCLUDE)/.

install-library:
			@echo "[*] Copying AISignal library"
			cp $(LIB_NAME) $(INSTALL_LIB)/.

install-sources:
			@echo "[*] Copying AISignal sources"
			mkdir -p $(INSTALL_SOURCES)
			cp -r $(PATH_SOURCE)/* $(INSTALL_SOURCES)/.

install:
			@echo "---"
			@echo "Installing libAISignal version $(LIB_VERSION)"
			@echo "Library installation path: $(INSTALL_LIB)"
			@echo "Headers installation path: $(INSTALL_INCLUDE)"
			@echo "---"
			make --no-print-directory install-headers
			make --no-print-directory install-library
			@echo "Done."


# Removing & cleaning rules
# =========================
remove-headers:
			@echo "[*] Removing AISignal includes & headers"
			if [ -e $(INSTALL_INCLUDE) ]; then rm -r -I $(INSTALL_INCLUDE); fi

remove-library:
			@echo "[*] Removing AISignal library"
			if [ -e $(INSTALL_LIB)/$(LIB_NAME) ]; then rm -i $(INSTALL_LIB)/$(LIB_NAME); fi

remove-sources:
			@echo "[*] Removing AISignal sources"
			if [ -d $(INSTALL_SOURCES) ]; then rm -r -I $(INSTALL_SOURCES); fi

remove:
			@echo "---"
			@echo "Removing libAISignal"
			@echo "Library installation path: $(INSTALL_LIB)"
			@echo "Headers installation path: $(INSTALL_INCLUDE)"
			@echo "---"
			make --no-print-directory remove-headers
			make --no-print-directory remove-library
			@echo "Done."

clean:
			if [ -e $(LIB_NAME) ]; then rm $(LIB_NAME); fi
