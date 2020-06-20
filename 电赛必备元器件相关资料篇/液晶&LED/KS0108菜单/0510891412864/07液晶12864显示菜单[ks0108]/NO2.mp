NOTE: AVR Code addresses are word addresses
All other addresses (including ones in FLASH) are byte addresses

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                           idata   0054   0140 =    320. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0054  __idata_start
       0194  __idata_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   0194   04AA =   1194. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       00CA  __start
       00CA  __text_start
       00EC  _exit
       00ED  _delay_1us
       00EF  _delay_nus
       00FE  _delay_1ms
       0108  _delay_nms
       0117  _delay
       012F  _Lcd_Write_Byte
       013C  _Lcd_Write_COM
       0148  _set_page_L
       014F  _set_page_R
       0156  _set_col_addr_L
       015D  _set_col_addr_R
       0164  _Lcd_Set_X_Y
       017B  _Lcd_Character_16X16
       01CE  _key_press
       01DF  _key_scan
       0216  _Lcd_Function
       02D2  _main
       0308  pop_xgset003C
       030D  pop_xgset00FC
       0314  push_xgsetF0FC
       0316  push_xgset30FC
       0318  push_xgset00FC
       031A  push_xgset003C
       031F  __text_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            data   0060   0140 =    320. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0060  _time_logo
       0060  __data_start
       0100  _menu
       01A0  __data_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                          vector   0000   0004 =      4. bytes (abs,ovr,rom)

Files Linked      [ module(s) ]

C:\iccv7avr\lib\crtatmega.o	[ crtatmega.s ]
NO2.o	[ NO2.C ]
<library>	[ gpop2.s, gpop3.s, gpush.s ]

User Global Definitions

ram_end = 0x45f
hwstk_size = 0x1e

User Base Address Definitions

func_lit = 0x54
data = 0x60
eeprom:0.512

