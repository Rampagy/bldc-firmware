import pandas as pd
import os
import matplotlib.pyplot as plt

FILE_PATH = './48v_tests'

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

MeterRPM = [5500, 5499, 5500, 5501, 5500]

MeterPower = [
    (113.5, 'C0'), # watts (ars)
    (112.5, 'C1'), # watts (null v0)
    (112.5, 'C2'), # watts (null v7)
    (112.5, 'C3'), # watts (v0_odd_v7_even)
    (110.0, 'C4'), # watts (v7_odd_v0_even)
]

if __name__=='__main__':
    test_files = os.listdir(FILE_PATH)
    test_names = []
    fig, axes = plt.subplots(1)

    for i, f in enumerate(test_files):
        test_name = os.path.splitext(f)[0]
        df = pd.read_csv(os.path.join(FILE_PATH, f), names=HEADERS, delimiter=';', index_col=False)

        # get average watts/rpm in the last 2 sec of each test
        power_time = df[df['Time'] > 121]
        ave_power = (power_time['RPM']/power_time['Power']).mean()

        # get final average temperature
        temp_time = df[df['Time'] > 121]
        final_temp = temp_time['MOSFET Temp 1'].mean()

        print('{:>16s} :: {:5.1f} RPM/watt ({:4.1f} °C)'.format(test_name, ave_power, final_temp))

        test_names += [test_name]
        plot_time = df[df['Time'] > 20]
        axes.plot(plot_time['RPM'].to_list(), plot_time['Power'].to_list())


    for i, [p, c] in enumerate(MeterPower):
        axes.plot(MeterRPM[i], [p], 'p', color=c, markersize=10)

    axes.set_title('RPM/watt for each commutation technique')
    axes.set_xlabel('RPM')
    axes.set_ylabel('Watts')
    axes.legend(test_names)
    axes.grid()
    axes.set_ylim(bottom=0, top=100)
    axes.set_yticks(range(0, 150, 10))



    plt.subplots_adjust(left=0.1,
                    bottom=0.1,
                    right=0.9,
                    top=0.9,
                    wspace=0.5,
                    hspace=0.5)
    plt.show()