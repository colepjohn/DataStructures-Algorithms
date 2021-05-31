# Array and Linked List

This was the first issued lab for the course. It's purpose was to refresh the memories of all students and begin the 
introduction to the second part of Data Structures and Algorithms. This folder holds the files for Fruit1, the program 
that utilizes an array list, and Fruit2, the program that utilizes a linked list, as well as three sample lists to run.

## Prompt
Each day, the folks at Josephine's Juice Bar buy a lot of fruit for their smoothies. Management would like to know the 
quantity purchased of each different type of fruit (all fruit is purchased by weight) and at what cost plus the total cost. 
Your job is write two C++ programs that solve the problem based respectively on an array list and a linked list. Kidding 
aside, the real task, of course, is to get you back in the programming saddle before we get deep into new data structures 
and algorithms. Those of you not used to "word problems" will also gain some experience deciphering what lab assignments 
ask you to do.

## Purpose
While both Fruit1 and Fruit2 accomplish the same thing and produce the same output, they do so using different data structures.
These programs read in data from a file that holds details of the name of a fruit, the cost of a single fruit, and the quantity
purchased. It then forms either an array list or linked list and stores all of the information included the calculated total cost.
Both programs implement three separate options for the user to utilize in sorting the data provided through a file. These options
include **inorder**, **sortone**, and **sortall**. These display the data in order by total cost of the fruit, alphabetical order
while combining duplicate fruits, and alphabetical order without combining duplicate fruits.

### Usage
**Command:** Fruit1[2] -inorder|sortone|sortall file

### Example
**Command:** Fruit2 -sortone list1.txt
'''
apples_gala.........  6.25 x 1.21 =    7.56  :     7.56
apples_jazz.........  4.39 x 2.69 =   11.81  :    19.37
bananas.............  2.88 x 0.49 =    1.41  :    20.78
blueberries.........  3.85 x 2.50 =    9.62  :    30.41
mango...............  4.07 x 1.20 =    4.88  :    35.29
melon_cantaloupe....  2.60 x 2.99 =    7.77  :    43.07
oranges_honeybell...  8.42 x 1.08 =    9.09  :    52.16
oranges_naval.......  2.63 x 0.99 =    2.60  :    54.76
raspberries.........  4.76 x 3.25 =   15.47  :    70.23
'''
