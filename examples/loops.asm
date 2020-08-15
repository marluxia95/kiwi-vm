.code   
    MOV 2 1     ; Moves address 2 to register 1
    ACC 2       ; Moves 2 to accumulator
    ADD 2       ; Adds ACC to register 2
    JNE 16 3    ; If ACC is not equal to 16 then goto 3
    HLT         ; End 
    
