import commutation_techniques
import matplotlib.pyplot as plt
import csnake # python -m pip install csnake
import os

if __name__ == '__main__':

    # generate look up tables for spwm and svpwm
    angles = []
    spwm_lut = []
    svpwm_lut = []
    for angle in range(360):
        angles += [angle]

        spwm_lut += [int(round(commutation_techniques.spwm(angle, 100, 100)[0]))]
        svpwm_lut += [int(round(commutation_techniques.svpwm(angle, 100, 100)[0]))]


    # recreate each phase for the commutation method using the lookup tables to ensure they are correct
    spwm_phases = []
    svpwm_phases = []
    for angle in range(360):
        # always 0% field weakening
        a = (angle + 90) % 360
        b = (angle + 90 + 240) % 360
        c = (angle + 90 + 120) % 360

        spwm_phases += [[spwm_lut[a], spwm_lut[b], spwm_lut[c]]]
        svpwm_phases += [[svpwm_lut[a], svpwm_lut[b], svpwm_lut[c]]]

    # look up tables
    spwm_lut_var = csnake.Variable('SPWM_LUT', 'uint8_t', value=spwm_lut)
    svpwm_lut_var = csnake.Variable('SVPWM_LUT', 'uint8_t', value=svpwm_lut)
    
    # duty cycle structure
    duty_cycle_struct = csnake.Struct('Duty_Cycle_Struct', typedef=True)
    [duty_cycle_struct.add_variable( (i, 'uint8_t') ) for i in ['a', 'b', 'c']]

    # commutation type
    comm_type_enum = csnake.Enum('commutation_type_enum', typedef=True)
    comm_type_enum.add_values( ['spwm_e', 'svpwm_e'] )

    # variables
    direct_axis_var = csnake.Variable('direct_axis_angle', 'uint16_t')
    throttle_var = csnake.Variable('throttle_perc', 'int8_t')
    field_weakening_var = csnake.Variable('field_weakening_perc', 'uint8_t')
    commutation_type_var = csnake.Variable('commutation_type', comm_type_enum.name)

    # create function
    pwm_lookup_func = csnake.Function('GetPWMDutyCycles', return_type=duty_cycle_struct.name)
    pwm_lookup_func.add_argument( direct_axis_var )
    pwm_lookup_func.add_argument( throttle_var )
    pwm_lookup_func.add_argument( field_weakening_var )
    pwm_lookup_func.add_argument( commutation_type_var )

    # generate header file
    cw = csnake.CodeWriter()
    cw.add_autogen_comment(os.path.basename(__file__))
    cw.add_line()
    cw.start_if_def('COMM_LOOK_UP_TABLE_H', invert=True)
    cw.add_define('COMM_LOOK_UP_TABLE_H')
    cw.add_line()
    cw.add_line(comment='INCLUDES')
    cw.include("<stdint.h>")
    cw.add_line()
    cw.add_line(comment='TYPE DEFINITIONS')
    cw.add_enum(comm_type_enum)
    cw.add_line()
    cw.add_struct(duty_cycle_struct)
    cw.add_line()
    cw.add_line(comment='GLOBAL VARIABLES')
    cw.add_line()
    cw.add_line(comment='FUNCTION PROTOTYPES')
    cw.add_function_prototype(pwm_lookup_func)
    cw.add_line()
    cw.end_if_def()
    cw.write_to_file('../include/CommutationLookupTable.h')

    # generate ch file for the lookup tables
    cw = csnake.CodeWriter()
    cw.add_autogen_comment(os.path.basename(__file__))
    cw.add_line()
    cw.add_line(comment='Lookup table used for Sinusoidal PWM commutation')
    cw.add_variable_initialization(spwm_lut_var)
    cw.add_line()
    cw.add_line(comment='Lookup table used for Space Vector PWM commutation: https://youtu.be/5eQyoVMz1dY')
    cw.add_variable_initialization(svpwm_lut_var)
    cw.add_line()
    cw.write_to_file('../include/CommutationLookupTable.ch')

    # plot the results
    fig, axs = plt.subplots(3)

    axs[0].set_title('Lookup Table Results')
    axs[0].plot(angles, spwm_lut, 'b-', angles, svpwm_lut, 'g--')
    axs[0].legend(['spwm', 'svpwm'])
    axs[0].set_xlabel('Direct Axis Angle, deg')
    axs[0].set_ylabel('High Side Transistor Duty Cycle, percent')

    axs[1].set_title('3 phase recreation using the SPWM LUT')
    axs[1].plot(angles, spwm_phases)
    axs[1].legend(['a', 'b', 'c'])
    axs[1].set_xlabel('Direct Axis Angle, deg')
    axs[1].set_ylabel('High Side Transistor Duty Cycle, percent')

    axs[2].set_title('3 phase recreation using the SVPWM LUT')
    axs[2].plot(angles, svpwm_phases)
    axs[2].legend(['a', 'b', 'c'])
    axs[2].set_xlabel('Direct Axis Angle, deg')
    axs[2].set_ylabel('High Side Transistor Duty Cycle, percent')

    plt.show()