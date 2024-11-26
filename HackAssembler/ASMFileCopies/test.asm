@6	//set op1 value
D=A
@op1
M=D

@7	//set op2 value
D=A
@op2
M=D

@op1	//set count to value of op1, will be used to count iterations of mul loop
D=M
@Count
M=D

@sum	//init sum to 0
M=0

(loop)
@op2	//get op2 into D
D=M
@sum
M=M+D	//add it to sum

@Count	//get addr of loop count
M=M-1	//decrement it

@Count	//get count into D
D=M

@loop	//if D is not at zero, loop again
D;jne

(END)	//infinite loop ==> END
@end
D;JMP