@call \DIY\SetVars.cmd
 echo Building...
 make -f midigrids\makefile %1 > nul
 if errorlevel 1 goto EOF
 if {%1} == {clean} goto EOF
:avr-objdump -h -S build\midigrids\midigrids.elf > build\midigrids\midigrids.lst
 set path=%path%;C:\Program Files (x86)\Atmel\AVR Tools\STK500
 echo Flashing...
 stk500 -cUSB -dATmega328p -I4Mhz -e -ifbuild\midigrids\midigrids.hex -pf -vf
:EOF
 echo DONE!