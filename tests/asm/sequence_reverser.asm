; Read a sequence of positive numbers (terminated by a zero) and
; output the sequence in reverse order (with zero terminator). Repeat.

@input:
subleq @tmp, @IN
subleq @tmp2, @tmp, @start_pop_loop
subleq @count, @n_one
subleq @tmp, @tmp

@stack_push:
subleq @stack, @tmp2
subleq @tmp, @stack
subleq @tmp2, @tmp2

; Modify the instruction at @stack_push (increment target address)
subleq @stack_push, @n_one
subleq @tmp, @tmp, @input

; Prepare to start popping values off of the stack by copying the
; current stack position to @stack_pop+1
@start_pop_loop:
subleq @stack_push, @one
subleq @tmp, @stack_push
subleq @stack_pop+1, @stack_pop+1
subleq @clear_stack+1, @clear_stack+1
subleq @clear_stack, @clear_stack
subleq @stack_pop+1, @tmp
subleq @clear_stack+1, @tmp
subleq @clear_stack, @tmp

; Read a value from the stack (note: the second address of this
; instruction is repeatedly decremented)
@stack_pop:
subleq @OUT, 0
@clear_stack:
subleq 0, 0

; Decrement stack address in the instruction at @stack_pop
subleq @stack_pop+1, @one
subleq @count, @one, @prepare
subleq @clear_stack+1, @one
subleq @clear_stack, @one
subleq @tmp, @tmp, @stack_pop

; Set stack for push after pop
@prepare:
subleq @OUT, @tmp
subleq @tmp, @stack_pop+1
subleq @stack_push, @stack_push
subleq @stack_push, @tmp
subleq @stack_push, @n_one

subleq @tmp, @tmp, @input

; Constants
@one: .data 1
@n_one: .data -1

; Variables
@tmp: .data 0
@tmp2: .data 0
@count: .data 0

; Base of stack (stack will grow to larger addresses, so no varaibles
; should be placed after this one)
@stack: .data 0

