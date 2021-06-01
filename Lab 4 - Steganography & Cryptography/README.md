# Steganography

This lab showcases work done with binary file I/O and outputting PPM images. The folder consists of files Crypto2 and Crypto3,
which implement the programs main function and an extra encoding respectively. The folder also consists of header and declaration
files PPM and Rnumgen.

## Prompt
In this lab you will write a program for embedding ASCII text within a PPM image using a randomized algorithm for selecting 
where to place the bits that make up the ASCII characters. We will call the program "crypto" which makes it sound as if it 
is based on cryptography. In reality, it is a case of steganography which according to Wikipedia "is the art or practice of 
concealing a file, message, image, or video within another file, message, image, or video." Either way, the program is actually 
pretty cool.

Before you get any ideas to turn it into an app that will make your rich, be forewarned that no attempt has been made to research 
the literature on the subjects of steganography and crytopgraphy. In particular, somebody may hold a patent on a similar idea. 
Besides, you would have to support JPG, HEIC and other standard image formats that use compression which adds substantial complications. 
The goal of the exercise is simply to have you work with formatted and binary file I/O as well as random numbers.

## Implementation
This program introduces the idea of Steganography and does so by implementing a randomized algorithm for selecting bits within a PPM image
and encoding an ASCII character to be decoded later. The program can also decode PPM images that consist of such secret messages. The main
takeaways include the ability to work with binary file I/O, PPM images, and creation of a random number generator based on distribution of 
user data.

### Usage
**Command:** ./Crypto2|Crypto3 -encode|decode [-seed=N] [-key="text"] file

### Example
**Command:** ./Crypto3 -encode -seed=120 -key="secret key" image.ppm
