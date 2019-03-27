# Forma:
# target : dependencies
#<TAB>	comando

EXE := foguete

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

# Diretorio com headers
CPPFLAGS += -I./include/
# Compiler flags
CFLAGS += -Wall -Werror

# Linker flags i.e -L
LDFLAGS +=
# Libraries i.e -l
LDLIBS += -lGL -lGLU -lglut -lm

# .PHONY - Impede que arquivos com o mesmo nome atrapalhem o make
.PHONY: all clean

# $@ - Contem o nome do target
# $^ - Contem todas as dependencias
# $< - Contem a primeira dependencia

all: $(EXE)

# Linka os arquivos objetos e produz executável
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Obtem arquivos .o
# -c - Produz apenas codigo compilado(object files), sem linkar-los
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJ) $(EXE)
	
