Hey Gökce

This is my interpretation of the FrankenText program


## Overview:
------------

This program:

- Reads the text of Frankenstein (embedded at compile time).

- Tokenizes it into words.

- Builds a successor table showing which words follow which.

- Generates random sentences using this data (a simple Markov chain).

Example output:

My dear Victor Frankenstein in the mountains and when it appeared to them it was a moment of joy and wonder?

Excellent! I will not hear you now!

## How It Works:
----------------

- Non-printable characters are replaced with spaces.

- Each unique word is stored in a token list.

- Successor relationships between words are recorded.

- Sentences start with a random capitalized word and end when a ., ?, or ! is found.

## File Structure:
------------------

main.c           # Main program
pg84.txt         # Embedded text of Mary Shelley's Frankenstein
