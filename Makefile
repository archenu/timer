Target =  test.exe
all: 
	gcc *.c -Wall -o  test.exe

clean:
	del  test.exe *.o