.text 

addi $a0, $0, 0			# Insert inout value in n
addi $v0, $0, 1			# Variable to count up the factorial
addi $a2, $0, 0			
addi $a3, $0, 0


while_loop: 
beq $v0, $a1, factorial_done		# Condition in the main structure
addi $a1, $a1, 1			# Add 1 to the factorial


inne_loop: 
beq $a2, $a1, inne_done
add $0, $0, $0				#NOP
addi $v0, $v0, 1
beq $0, $0, inner_loop
add $0, $0, $0				#NOP
inner_done


beq $0, $0, while_loop
add $0, $0, $0				#NOP
factorial_done:


stop_loop: 				# Loop that jumps to itself forever. The stop. 
beq $0, $0, stop_loop
add $0, $0, $0				#NOP
