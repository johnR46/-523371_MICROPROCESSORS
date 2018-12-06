		ORG	0000H
		MOV	R1,#80H
		MOV	R2,#0FFH
		MOV	R3,#00H  ; MIN
		; VALUE = R4
		
LOOP:		MOV	A,R4
		JB	R4,#0,$
		
		

		
		CJNE	R1,#0FFH,LOOP




		
		
		
		
		
		
		
		
		
		
		JMP	$
		END