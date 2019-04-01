main: 

li $t1, 10

loop: 
beq $t1, $t0, stop_loop
add $t0, $t0, 1
j loop

stop_loop: 
