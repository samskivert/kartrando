TARGET = kartrando
NAME = Kart Rando

CFLAGS = -Wall -g

all : $(TARGET).prc

$(TARGET).prc : code0001.$(TARGET).grc tFRM2702.bin
	@echo Building $(TARGET).prc
	@build-prc $(TARGET).prc "$(NAME)" KRnd *.grc *.bin 

$(TARGET)_ids.h: $(TARGET).rcp
	@echo Compiling resources...
	@pilrc -q -H $(TARGET)_ids.h $(TARGET).rcp

tFRM2702.bin: $(TARGET).rcp kartrando_icon.bmp
	@echo Compiling 2 resources...
	@pilrc -q -H $(TARGET)_ids.h $(TARGET).rcp

code0001.$(TARGET).grc: $(TARGET).c $(TARGET)_ids.h
	@echo Compiling source code...
	@m68k-palmos-coff-gcc $(CFLAGS) -c $(TARGET).c -o $(TARGET).o 
	@m68k-palmos-coff-gcc $(CFLAGS) $(TARGET).o -o $(TARGET)
	@m68k-palmos-coff-obj-res $(TARGET)

clean:
	rm *.bin *.grc *.o $(TARGET) $(TARGET).prc
