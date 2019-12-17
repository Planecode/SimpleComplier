@set INCPATH=F\masm32\include
@set LIBPATH=F\masm32\lib

ml -c -coff /I %INCPATH% %1.asm
link -subsystem:console -LIBPATH:%LIBPATH% %1.obj
