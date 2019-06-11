# Bits, Bytes and Integer

只有ENIAC使用十进制进行算术运算    
    
二进制 模拟信号 设定阈值可以起到类似降噪抗干扰的作用    

隐式表示数值集合 如：01000001 {0, 6}    
> & -> Intersection     
> | -> Union    
> ^ -> Symmetric difference    
> ~ -> Complement    
    
     
## &&, ||, !    
+ View 0 as "false"
+ Anything nonzero as "true"
+ Always return 0 or 1
+ **Early termination**
     
### Examples(char data type)
+ !0x41 -> 0x00
+ !0x00 -> 0x01
+ !!0x41 -> 0x01    
   
+ 0x96 && 0x55 -> 0x01
+ 0x96 || 0x55 -> 0x01
+ p && \*p (used to avoid null pointer accesss)

## Shift Operations
### Left Shift: x \<\< y
+ Fill with 0's on right   
+ Throw away extra bit on left   
### Right Shift: x >> y
+ Throw away extra bit on right   
+ **Logical shift**: Fill with 0's on left
+ **Arithmetic shift**: Replicate most significant bit on left
