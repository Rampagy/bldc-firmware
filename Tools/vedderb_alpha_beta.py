import commutation_techniques

if __name__=='__main__':
	# 96.5% @ 45
	print(commutation_techniques.ars_svpwm(45, 50, 100)) # alpha=0.70710678118, beta=0.70710678118
	print(commutation_techniques.spwm(45, 50, 100)) # alpha=0.70710678118, beta=0.70710678118