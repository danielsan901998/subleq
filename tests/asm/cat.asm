; Cat program
; Reads input and outputs it

@loop:
subleq @loop, @IN
subleq @OUT, @loop
subleq @loop, @loop, @loop  ; Reset @loop to zero, and jump to @loop
