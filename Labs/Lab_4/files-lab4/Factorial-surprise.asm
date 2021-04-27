# Initialize
	addi 	$a0, $zero, 5		# Change the immediate/3 to change function call argument
	
		
# Main factorial function
	beq	$a0, $zero, Zero	# Guard for 0!
	add	$v0, $a0, $zero		# Value out is initially the argument
	add	$a1, $a0, $zero
	
Factorial:
	addi	$a1, $a1, -1
	beq	$a1, 1,	Stop
	mul	$v0, $v0, $a1
	beq	$zero, $zero, Factorial
	
		


# If Zero
Zero:
	addi	$v0, $zero, 1
	

# Stoploop
Stop:
	beq	$0, $0, Stop
	nop
