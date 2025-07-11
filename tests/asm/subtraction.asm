; Subtraction test
; Read two numbers (A, then B) and output A minus B. Repeat.
@loop:
subleq @tmp, @IN
subleq @tmp+1, @IN
subleq @tmp, @tmp+1
subleq @OUT, @tmp
subleq @tmp, @tmp  ; Reset @tmp to zero
subleq @tmp+1, @tmp+1, @loop  ; Reset @tmp+1 to zero, and jump to @loop

@tmp: .data 0

