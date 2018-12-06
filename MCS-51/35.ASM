	ORG	0000H
	MOV	DPTR,#2300H
	MOV 	R0,DPH
        MOV 	R1,DPL
        MOV	DPTR,#1000H
	MOV 	R2,DPH
        MOV 	R3,DPL
LOOP:	MOVX	A,@DPTR
	INC	DPTR
	MOV 	R2,DPH
        MOV 	R3,DPL
        MOV	DPH,R0
        MOV	DPL,R1
        MOVX	@DPTR,A
        INC	DPTR
        MOV 	R0,DPH
        MOV 	R1,DPL
        MOV	DPH,R2
        MOV	DPL,R3
        CJNE	R2,#11H,LOOP
        CJNE	R3,#20H,LOOP
        
	JMP	$
	END