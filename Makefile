hello:
	echo "Hello World"

mallocMain: mallocMain.c
	gcc -o mallocMain mallocMain.c

mallocMainWithCustomMalloc: customMalloc.c mallocMain.c
	gcc -o mallocMain customMalloc.c mallocMain.c

run: mallocMain.c
	./mallocMain

clean:
	rm memoryAllocator
