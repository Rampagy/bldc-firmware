import pandas as pd
import os
import matplotlib.pyplot as plt

FILE_PATH = './encoder_v0v7_speed_ramp'

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


    axes.set_title('RPM/watt for each commutation technique (with hall effect and sampling in v7 and v0)')
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