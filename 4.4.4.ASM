				ORG 	0000H
				; first
				MOV	DPTR,#9001H
				MOVX	A,@DPTR
				MOV	R0,A
				INC	DPTR
				MOVX	A,@DPTR
				MOV	R1,A
				INC	DPTR
				MOVX	A,@DPTR
				MOV	R2,A
				INC	DPTR
				MOVX	A,@DPTR
				MOV	R3,A
				
				;last
				MOV	DPTR,#9011H
				MOVX	A,@DPTR
				MOV	R4,A
				INC	DPTR
				MOVX	A,@DPTR
				MOV	R5,A
				INC	DPTR
				MOVX	A,@DPTR
				MOV	R6,A
				INC	DPTR
				MOVX	A,@DPTR
				MOV	R7,A
				
				; DPTR SUM
				MOV	DPTR,#9024H
				
				
				MOV	A,R3
				ADDC	A,R7
				MOVX	@DPTR,A
				MOV	DPTR,#9023H
				
				MOV	A,R2
				ADDC	A,R6
				MOVX	@DPTR,A
				MOV	DPTR,#9022H
				
				
				MOV	A,R1
				ADDC	A,R5
				MOVX	@DPTR,A
				MOV	DPTR,#9021H
				
				
				MOV	A,R0
				ADDC	A,R4
				MOVX	@DPTR,A
				MOV	DPTR,#9020H	
				
				
				
				
				
			
				
				
				
				
				
				
				
				
				
				
				
				JMP	$
				END