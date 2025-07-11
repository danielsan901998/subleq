	; Addition test (A + B))
@loop:
subleq @tmp, @IN
subleq @tmp, @IN
subleq @OUT, @tmp
subleq @tmp, @tmp, @loop  ; Reset @tmp to zero, and jump to @loop

@tmp: .data 0
