TARGET = kartrando
NAME = Kart Rando

CFLAGS = -Wall -g

all : $(TARGET).prc

$(TARGET).prc : code0001.$(TARGET).grc tfrm03e8.bin
	build-prc $(TARGET).prc "$(NAME)" KRnd *.grc *.bin 

$(TARGET)_ids.h: $(TARGET).rcp
	pilrc -q -H $(TARGET)_ids.h $(TARGET).rcp

tfrm03e8.bin: $(TARGET).rcp
	pilrc -q -H $(TARGET)_ids.h $(TARGET).rcp

code0001.$(TARGET).grc: $(TARGET).c $(TARGET)_ids.h
	m68k-palmos-coff-gcc $(CFLAGS) -c $(TARGET).c -o $(TARGET).o 
	m68k-palmos-coff-gcc $(CFLAGS) $(TARGET).o -o $(TARGET)
	m68k-palmos-coff-obj-res $(TARGET)

clean:
	rm *.bin *.grc *.o $(TARGET) $(TARGET).prc
