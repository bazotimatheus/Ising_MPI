# Compilador e flags
CC=mpicc
CFLAGS=-I. -O3 -w -fexpensive-optimizations -m64 -foptimize-register-move -funroll-loops -ffast-math -mtune=native -march=native
LIBS=-lm

# Diretorios
SDIR=src
LDIR=lib
ODIR=obj
DDIR=data

# Seta os arquivos fontes e gera os objetos
_SRCS=ising.c main.c
SRCS=$(patsubst %,$(SDIR)/%,$(_SRCS))

_DEPS=ising.h
DEPS=$(patsubst %,$(LDIR)/%,$(_DEPS))

_OBJS=$(_SRCS:.c=.o)
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o:$(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

# Cria o executavel "ising"
ising: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Limpa a casa
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(DDIR)/data.out $(DDIR)/*.png *~ ising
	