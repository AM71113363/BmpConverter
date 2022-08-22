# Project: BmpConverter

CC   = gcc.exe -s
WINDRES = windres.exe
RES  = obj/main.res
OBJ  = obj/main.o obj/operation.o obj/SaveAs.o obj/display.o obj/hashmap.o $(RES)
LINKOBJ  = obj/main.o obj/operation.o obj/SaveAs.o obj/display.o obj/hashmap.o $(RES)
LIBS =  -L"C:/Dev-Cpp/lib" -mwindows  
INCS =  -I"C:/Dev-Cpp/include" 
BIN  = BmpConverter.exe
CFLAGS = $(INCS)  
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before BmpConverter.exe all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o "BmpConverter.exe" $(LIBS)

obj/main.o: main.c
	$(CC) -c main.c -o obj/main.o $(CFLAGS)

obj/operation.o: operation.c
	$(CC) -c operation.c -o obj/operation.o $(CFLAGS)

obj/SaveAs.o: SaveAs.c
	$(CC) -c SaveAs.c -o obj/SaveAs.o $(CFLAGS)

obj/display.o: display.c
	$(CC) -c display.c -o obj/display.o $(CFLAGS)

obj/hashmap.o: hashmap.c
	$(CC) -c hashmap.c -o obj/hashmap.o $(CFLAGS)

obj/main.res: main.rc 
	$(WINDRES) -i main.rc --input-format=rc -o obj/main.res -O coff 

