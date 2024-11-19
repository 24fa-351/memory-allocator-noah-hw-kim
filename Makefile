hello:
	echo "Hello World"

systemMallocTest: memtest.c
	gcc -DSYSTEM_MALLOC -o test_system_malloc memtest.c

customMallocTest: malloc.c memtest.c
	gcc -o test_custom_malloc malloc.c memtest.c

runSystem: systemMallocTest
	./test_system_malloc

runCustom: customMallocTest
	./test_custom_malloc

clean:
	rm -f test_system_malloc test_custom_malloc
