# $a0 = n
# $v0 = result
# $a1 = product
# $a2 = i
# $a3 = j
# $v1 = p

#####################| Variables |#####################
addi $a0, $0, 3		          # n = 3
addi $v0, $0, 1		          # result = 1
addi $v1, $a0, 1	          # p = n + 1
addi $a2, $0, 1		          # i = 1

#####################| Main Loop |#####################
loop:
  beq $a2, $v1, done	      	# i != p
	addi $a2, $a2, 1	        # i++
	addi $a1, $0, 0	         	# product = 0
	addi $a3, $0, 1	        	# j = 1
	
#####################| Inner Loop |#####################
mul:
  beq $a3, $a2, mulDone 	  # j != i
	addi $a3, $a3, 1	        # j++
	add $a1, $a1, $v0	        # product += result
	beq $0, $0, mul		        # jump back to mul

mulDone:
  addi $v0, $a1, 0		      # result = produkt of mul
	beq $0, $0, loop		      # jump back to loop

done:
  beq $0, $0, done		      # infinitum loopius
