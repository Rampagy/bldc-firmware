import pandas as pd
import os
import matplotlib.pyplot as plt

FILE_PATH = './tests'

HEADERS = [
    'Time',
    'Power',
    'Current In',
    'Motor Current',
    'Voltage In',
    'RPM',
    'MOSFET Temp',
    'Motor Temp',
    'Duty Cycle',
    'MOSFET Temp 1',
    'MOSFET Temp 2',
    'MOSFET Temp 3'
]

if __name__=='__main__':
    test_files = os.listdir(FILE_PATH)
    fig, axes = plt.subplots(len(test_files))

    for i, f in enumerate(test_files):
        test_name = os.path.splitext(f)[0]
        df = pd.read_csv(os.path.join(FILE_PATH, f), names=HEADERS, delimiter=';', index_col=False)

        # get average watts/rpm in the last 2 sec of each test
        power_time = df[df['Time'] > 18]
        ave_power = (power_time['RPM']/power_time['Power']).mean()

        # get final average temperature
        temp_time = df[df['Time'] > 19.5]
        final_temp = temp_time['MOSFET Temp 1'].mean()

        print('{:>16s} :: {:5.1f} RPM/watt ({:4.1f} °C)'.format(test_name, ave_power, final_temp))

        axes[i].plot(df['Time'], df['MOSFET Temp 1'], df['Time'], df['MOSFET Temp 2'], df['Time'], df['MOSFET Temp 3'])
        axes[i].set_title(test_name)
        axes[i].set_xlabel('Time, s')
        axes[i].set_ylabel('FET Temperature, C')
        axes[i].grid()
        axes[i].set_ylim(bottom=18, top=24.5)



    plt.subplots_adjust(left=0.1,
                    bottom=0.05,
                    right=0.9,
                    top=0.95,
                    wspace=0.5,
                    hspace=0.5)
    plt.show()