# Bits, Bytes and Integers cont

## Unsigned Addition
Operands: w bits    
True Sum: w+1 bits    
Discard Carry: w bits    

### Standard Addition Function
+ Ignores carry output
### Implements Modular Arithmetic
+ s = UADDw(u, v) = u + v mod 2^w
+ Overflow (P61)

## Two's Complement Addition
Operands: w bits    
True Sum: w+1 bits    
Discard Carry: w bits    
	
	 1101 -3
	+0101  5
	---------
	 0010  2

	 1011 -5
	+0011  3
	---------
	 1110 -2 

	 1101 -3
	+1010 -6
	---------
	 0111  7  (Negative Overflow)

	 0111  7
	+0101  5
	---------
	 1100 -4  (Positive Overflow)
### TAdd Overflow
**Functionality**    
+ True sum requires w+1 bits
+ Drop off MSB
+ Treat remaining bits as 2's comp. integer
+ Overflow(P/N) (P64)

## Multiplication
**Exact results can be bigger than w bits, so, maintaining exact results.**      
### Unsigned case
+ UMULTw(u, v) = u * v mod 2^w
