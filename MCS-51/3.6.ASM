	ORG	0000H
	MOV	DPTR,#1000H
	
	
		
LOOP:	MOVX	A,@DPTR
	MOV	@R0,A
	INC 	DPTR
	INC	R0
        MOV 	R2,DPH
        CJNE 	R2,#11H,LOOP
        MOV 	R3,DPL
        CJNE 	R3,#20H,LOOP
        

	JMP	$
	END