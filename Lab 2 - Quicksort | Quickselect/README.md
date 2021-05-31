# Quicksort and Quickselect

This lab introduces the algorithms quicksort and quickselect. This folder holds three sample text files to test the code
as well as the program that implements the algorithms, Qsort.

## Prompt
Another week, another sorting assignment. Similar to Lab 1, you will define a class for storing multi-column data consisting 
of a firstname, a lastname, and a phone number (all made up, but you knew that), read unspecified amounts of such data from 
a given file and apply the std::sort algorithm before you print the result to stdout. Then the real work begins by you 
implementing a variant of the quicksort algorithm described in class that in its basic form replaces std::sort and in a more 
advanced version based on the quickselect algorithm that allows partitioning of the data followed by sorting within a given range.

## Implementation
Qsort implements the algorithms **Quicksort** and **Quickselect**, a variant of quicksort. Rather than a median-of-three pivot,
the quicksort in my program operates by choosing a random index within the range and partitioning the data from there. Quickselect
operates basically the same way, but allows the user to pick a range of numbers to sort, rather than sorting the entire list.

### Usage
**Command:** ./Qsort -stl | -rpivot [k0 k1] file

### Example
**Command:** ./Qsort -stl list1.txt
```
HULL, THAD              708-807-6757
JOHNSTON, WILFRED       582-126-8861
NGUYEN, KAYLA           484-322-1527
PERKINS, THEODORE       723-668-3397
PERRY, BRUCE            540-916-2956
RIVERS, VALENTIN        726-204-2377
SALAZAR, STEPHAN        415-413-5058
SPENCE, LEVI            985-365-7415
WILKINS, ISRAEL         938-701-1455
WILKINS, JORDAN         938-701-1455
WITT, CANDACE           250-939-5404
```
