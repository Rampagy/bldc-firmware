import pandas as pd
import os
import matplotlib.pyplot as plt

FILE_PATH = './30_min_temp_rise'

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

        # get final average temperature
        final_temp_time = df[df['Time'] > 1750]
        final_temp = final_temp_time['MOSFET Temp'].mean()
        
        start_temp_time = df[df['Time'] < 2]
        start_temp = start_temp_time['MOSFET Temp'].mean()

        print('{:>16s} :: {:4.1f} °C -> {:4.1f} °C  ({:4.1f} delta °C)'.format(test_name, start_temp, final_temp, final_temp - start_temp))

        test_names += [test_name]
        plot_time = df[df['Time'] > 1]
        axes.plot(plot_time['Time'].to_list(), plot_time['MOSFET Temp'].to_list()) # - start_temp)


    axes.set_title('Temperature rise over time')
    axes.set_xlabel('Time, sec')
    axes.set_ylabel('Delta Temperature, °C')
    axes.legend(test_names)
    axes.grid()
    axes.set_ylim(bottom=10, top=46)
    axes.set_yticks(range(10, 46, 5))


    plt.subplots_adjust(left=0.1,
                    bottom=0.1,
                    right=0.9,
                    top=0.9,
                    wspace=0.5,
                    hspace=0.5)
    plt.show()