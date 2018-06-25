all: ars.exe

ars.exe : main.obj ars.obj ansic.obj
    link /DEFAULTLIB:msvcrt.lib /NODEFAULTLIB:LIBC.lib /NODEFAULTLIB:libcmt.lib ars.obj main.obj ansic.obj

main.obj : main.c main.h ars.h
    cl /c /MD /O2 main.c
    
ars.obj : ars.c ars.h
    cl /c /MD /O2 ars.c
    
ansic.obj : ansic.c main.h ars.h
    cl /c /MD /O2 ansic.c
clean: 
    del *obj *exe LOG.log
    