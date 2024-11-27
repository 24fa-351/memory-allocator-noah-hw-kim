hello:
	echo "Hello World"

systemMallocTest: memtest.c
	gcc -DSYSTEM_MALLOC -o test_malloc memtest.c

customMallocTest: malloc.c memtest.c
	gcc -o test_malloc malloc.c memtest.c

customMallocHeapTest: mallocHeap.c memtest.c
	gcc -o test_malloc mallocHeap.c memtest.c

run:
	./test_malloc

clean:
	rm -f test_system_malloc test_custom_malloc
