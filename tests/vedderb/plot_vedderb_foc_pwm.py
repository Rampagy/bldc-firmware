import matplotlib.pyplot as plt
import glob

if __name__=='__main__':
	svm_methods = glob.glob('svm_*.csv')
	fig, axs = plt.subplots(len(svm_methods)-2)
	fig.tight_layout()


	for i, svm_method in enumerate(svm_methods):
		dutyA = 0
		dutyB = 0
		dutyC = 0
		with open(svm_method, 'r') as f:
			for j, line in enumerate(f):
				if j == 32552:
					dutys = line.strip().split(',')
					dutyA = float(dutys[0])
					dutyB = float(dutys[1])
					dutyC = float(dutys[2])

		# now recreate the pwm signal
		pwm_a = []
		pwm_b = []
		pwm_c = []
		for j in list(range(1000)) + list(range(1000, -1, -1)):
			if j < dutyA:
				pwm_a += [1]
			else:
				pwm_a += [0]

			if j < dutyB:
				pwm_b += [1]
			else:
				pwm_b += [0]

			if j < dutyC:
				pwm_c += [1]
			else:
				pwm_c += [0]


		timer_count = list(range(len(pwm_c)))
		axs[i].plot(timer_count, pwm_a, 'g--', timer_count, pwm_b, 'b--', timer_count, pwm_c, 'r--')
		axs[i].legend(['Phase A', 'Phase B', 'Phase C'])
		axs[i].set_ylabel('Transistor state')
		axs[i].set_xlabel('time (timer counts)')
		axs[i].set_title(svm_method.split('.')[0] + ' pwm waveform (PWMFullDutyCycle = 1000)')
		if i >=2:
			break

	plt.show()
