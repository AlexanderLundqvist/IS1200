# Initialize
	addi 	$a0, $zero, 5		# Change the immediate/3 to change function call argument
	add 	$a1, $a0, $zero		# a1 & a2 used for conting in the loops
	add	$a2, $a0, $zero
	add	$a3, $a0, $zero
	


		
# Main factorial function
	beq	$a0, $zero, Zero	# Guard for 0!
	add	$v0, $a0, $zero		# Value out is initially the argument
OuterLoop:
	addi	$a1, $a1, -1
	beq	$a1, 1, Stop
	add	$a2, $a1, $zero
	add	$a3, $v0, $zero		# Store sum from inner loop
	InnerLoop:
		add	$v0, $v0, $a3
		addi	$a2, $a2, -1
		
		beq	$a2, 1, OuterLoop
		
		beq	$zero, $zero, InnerLoop # Force innerLoop unless previous branch did not execute (Unconditional)
		


# If Zero
Zero:
	addi	$v0, $zero, 1
	

# Stoploop
Stop:
	beq	$0, $0, Stop
	nop
