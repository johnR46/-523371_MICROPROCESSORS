	ORG	0000H
	MOV	DPTR,#2300H
	
	
		
LOOP:	MOVX	@DPTR,A
	INC 	DPTR
	ADD	A,#02H	
        MOV 	R2,DPH
        CJNE 	R2,#11H,LOOP
        MOV 	R3,DPL
        CJNE 	R3,#20H,LOOP
        
	JMP	$
	END