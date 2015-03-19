; The base address of array a is in r1 
; The base address of array b is in r2
; The base address of array c is in r3

addi r13, r0, 8
LOOP: lw r4, 0(r1)		; Load from a(0) into r4
      lw r5, 0(r2)		; Load from b(0) into r5
      lw r6, 4(r1)		; Load from a(4) into r6
      lw r8, 4(r2)		; Load from b(4) into r8
      lw r9, 8(r1)		; Load from a(8) into r9
      lw r10, 8(r2)		; Load from b(8) into r10
      lw r11, 12(r1)	; Load from a(12) into r11
      lw r12, 12(r2)	; Load from b(12) into r12

      subi r13, r13, 4  ; Decrement the counter
      
      add r7, r4, r5	; r7 = r4 + r5
      sw  0(r3), r7		; store result back in array c

      add r7, r6, r8	; r7 = r6 + r8
      sw  4(r3), r7		; store result back in array c

      add r7, r9, r10	; r7 = r9 + r10
      sw  8(r3), r7		; store result back in array c

      add r7, r11, r12	; r7 = r11 + r12
      sw  12(r3), r7	; store result back in array c
      
      addi r1, r1, 16	; Shifts the array
      addi r2, r2, 16	; Shifts the array
      bnez r13, LOOP	; Branches if it isn't 0
      addi r3, r3, 16	; Shifts the array

sw 0(r3), r0 		    ; set c[0] to 0. (not important, just something to do after the loop.)

nop;
nop; 
nop;
nop;	
trap #0;
