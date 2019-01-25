all:sort

sort:slowsort.c fastsort.c 
	gcc -o runslow slowsort.c
	gcc -o runfast fastsort.c



runslow:sort
	./runslow

runfast:sort
	./runfast

runbenchmark:sort
	./runslow
	./runfast

clean:
	rm *.o runslow runfast runbenchmark *.a
