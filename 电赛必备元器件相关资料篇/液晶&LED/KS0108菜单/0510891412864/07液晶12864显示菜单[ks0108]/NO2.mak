CC = iccavr
CFLAGS =  -e -D__ICC_VERSION="7.14C" -DATMEGA -DATMega16  -l -g -Mavr_enhanced -Wf-str_in_flash -Wf-r20_23 
ASFLAGS = $(CFLAGS)  -Wa-g
LFLAGS =  -g -e:0x4000 -ucrtatmega.o -bfunc_lit:0x54.0x4000 -dram_end:0x45f -bdata:0x60.0x45f -dhwstk_size:30 -beeprom:0.512 -fihx_coff -S2
FILES = NO2.o 

NO2:	$(FILES)
	$(CC) -o NO2 $(LFLAGS) @NO2.lk   -lcatmegagr
NO2.o: C:\iccv7avr\include\iom16v.h C:\iccv7avr\include\macros.h
NO2.o:	D:\Proteus\Proteus练习\新建文件夹\NO2.C
	$(CC) -c $(CFLAGS) D:\Proteus\Proteus练习\新建文件夹\NO2.C
