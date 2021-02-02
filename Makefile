FLAVOURS = c cil dotnet java llvm python wasm
FLAVOUR = dotnet
TARGET = calc-$(FLAVOUR)
LIBS = -lm
CC = cc
CFLAGS = -g -Wall

all: $(TARGET)

OBJECTS = $(patsubst %.c, %.o, $(wildcard ./src/common/*.c)) $(patsubst %.c, %.o, $(wildcard ./src/$(FLAVOUR)/*.c))
HEADERS = $(wildcard /src/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f ./src/common/*.o
	-rm -f ./src/$(FLAVOUR)/*.o
	-rm -f $(TARGET)
