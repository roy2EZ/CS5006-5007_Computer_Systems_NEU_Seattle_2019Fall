1.How many items you sorted?
I sorted 500 numbers as small size, 5000 as med and 50000 as large.

2.Runtime result:
---------------------
SLOW SORT: INSERTION
---------------------
Large: 50000 elements    Running time: 2168.851000 ms 
Med:   5000  elements    Running time: 19.830000 ms 
Small: 500   elements    Running time: 0.201000 ms 
---------------------
FAST SORT: INSERTION
---------------------
Large: 50000 elements    Running time: 11.057000 ms 
Med:   5000  elements    Running time: 0.492000 ms 
Small: 500   elements    Running time: 0.060000 ms

3.Size choose:
3.1 What should be the difference between this sizes:
My answer: the difference of the size should make run time in different level.
3.2 What size for small is the smallest that make sence?
My answer: the smallest size should at least make running time 0.001 ms
3.3 What size for large makes sense?
My answer:I choose large size that makes the slow sort about 2000ms, which makes the slow sort obvious slower than fast sort, but not too long to run the sorting.

4.Is there a size of list where insertion sort is usually faster than quicksort?
My answer: when size is smaller than 10, insertion sort is usually faster than quicksort.
5.How do these generated times compare the expected/analyzed times (using asymptotic analysis)?
My answer:time complexity of  insertion sort is O(n*n), quick sort is O(n*logn). The generated times is matching the expected/analyzed times.
6.What was the hardest part of this assignment?
My answer: to me, the hardest part is to right the pseudocode into real C code. And also I need be more good at sorting algorithm for comprehend and applied.