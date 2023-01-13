import matplotlib.pyplot as plt

if __name__=='__main__':
	dutyA = []
	dutyB = []
	dutyC = []
	angles = []

	angle = 0
	with open('vedderb_foc.csv', 'r') as f:
		for line in f:
			dutys = line.strip().split(',')
			dutyA += [float(dutys[0])]
			dutyB += [float(dutys[1])]
			dutyC += [float(dutys[2])]

			angles += [angle]
			angle += 1


	fig, axs = plt.subplots(1)
	fig.suptitle('Commutation techniques')

	axs.plot(angles, dutyA, 'g-', angles, dutyB, 'b-', angles, dutyC, 'r-')
	axs.legend(['FOC Phase A', 'FOC Phase B', 'FOC Phase C'])
	axs.set_ylabel('High side transistor duty cycle (percent)')
	axs.set_xlabel('Direct axis angle (deg)')
	axs.set_title('Duty cycle for different commutation techniques')

	plt.show()
