import matplotlib.pyplot as plt
import glob

if __name__=='__main__':
	svm_methods = glob.glob('svm_*.csv')
	fig, axs = plt.subplots(len(svm_methods))
	fig.tight_layout()


	for i, svm_method in enumerate(svm_methods):
		angle = 0
		dutyA = []
		dutyB = []
		dutyC = []
		angles = []
		with open(svm_method, 'r') as f:
			for line in f:
				dutys = line.strip().split(',')
				dutyA += [float(dutys[0])]
				dutyB += [float(dutys[1])]
				dutyC += [float(dutys[2])]

				angles += [angle]
				angle += 0.05

		axs[i].plot(angles, dutyA, 'g-', angles, dutyB, 'b-', angles, dutyC, 'r-')
		axs[i].legend(['Phase A', 'Phase B', 'Phase C'])
		axs[i].set_ylabel('High side transistor\nduty cycle time')
		axs[i].set_xlabel('Direct axis angle (deg)')
		axs[i].set_title(svm_method.split('.')[0] + ' (PWMFullDutyCycle = 1000)')

	plt.show()
