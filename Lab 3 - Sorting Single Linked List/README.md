# Sorting Single Linked List
This lab introduced the concepts of sorting a single linked list and using smart pointers. Within the program,
I recreated mergesort, quicksort using a random pivot, and quicksort using a 3-way partitioning on random pivot.
The folder consists of the program Slist that implements these, an Slist header file that contains the declarations,
and two sample files to test the code.

## Prompt
In this lab you will continue sorting the multi-column data from Lab 2 which consisted of a firstname, a lastname, 
and a phone number. The main difference is that the data will now be stored in single linked list. In order to 
allow sorting by means of STL sorting algorithms (or any of the ones covered in class), you will embed a smart pointer 
subclass within the single linked list class and write the associated code for setting things up and applying the result.

## Implementation
Slist implements the algorithms **mergesort** and **quicksort**, as well as the concept of smart pointers. Building off
of Lab 2, I substitue the vectors used to store the data read in with linked lists. Thus, I accomplish sorting a single
linked list using whichever algorithm the user passes to the program.

### Usage
**Command:** ./Slist -mergesort|quicksort|qsort_r3 file

### Example
**Command:** ./Slist -mergesort repeat1.txt
```
ACEVEDO GAVIN           356-488-3678       86
ACEVEDO GAVIN           356-488-3678      257
ADKINS NOREEN           218-589-7364      266
AGUIRRE ROLANDO         232-480-9863       66
AGUIRRE ROLANDO         232-480-9863       99
AGUIRRE ROLANDO         232-480-9863      310
ALLISON CHELSEA         565-133-4367      184
ALSTON ELOY             625-705-2549      284
ALSTON ELOY             625-705-2549      291
ANTHONY EDMOND          498-558-5625      290
ASHLEY ROSALIND         381-587-7613      175
ASHLEY ROSALIND         381-587-7613      301
ATKINSON BETHANY        475-954-1105       92
AYALA DAMON             376-528-9382       43
AYALA DAMON             376-528-9382       81
AYALA DAMON             376-528-9382      117
AYALA DAMON             376-528-9382      318
BAILEY ROBERTO          846-171-8639      158
BAIRD LINWOOD           936-892-1659       27
BAIRD LINWOOD           936-892-1659       98
BAIRD LINWOOD           936-892-1659      190.......
```
