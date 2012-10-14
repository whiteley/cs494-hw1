Ethersend CS 494/594  
Homework #1  
Bart Massey

For your first assignment, you will write a C program that constructs Ethernet packets to send a file.

Your program should accept two arguments on the command line -- ethernet source and destination addresses in the standard format consisting of 12 hex digits separated by six colons, e.g. `01:23:45:67:89:ab`

The program should read a file from standard input, package it up into Ethernet frame in the format described in the book, and write the frames on standard output. Each frame should have the source and destination address, a type field indicating type `0xabcd`, and a four-byte block of zeros where the FCS should be. (Extra credit for putting a correct Ethernet CRC in there.)

The payload of each data frame should be the next portion of the file: the payload should be 1500 bytes.

The last data frame should be followed by a packet of maximum length containing the 32-bit checksum of the file contents, in network byte order (most significant byte first, i.e. "big-endian"). The checksum is computed by simply adding each byte of the file into a 32-bit register, discarding overflow.

You must submit the following:

Your C source code, together with any other files needed to try it out.

A README.homework file containing a writeup in plain text of not more than 1000 words describing what you did, how it worked, and anything else you think we should know.
