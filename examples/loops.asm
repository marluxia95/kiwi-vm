.code   
    MOV 2 1     ; Moves value 2 to register 1
    MOV 64 2     ; Moves value 6 to register 2
    ACC 2       ; Moves 2 to accumulator
    ADD 1       ; Adds ACC to register 2
    JNE 2 8     ; If ACC is not equal to R2 then goto 3
    HLT         ; End 
    

