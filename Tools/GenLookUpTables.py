import commutation_techniques
import matplotlib.pyplot as plt
import csnake # python -m pip install csnake
import os

if __name__ == '__main__':

    # generate look up tables for spwm and svpwm
    angles = []
    spwm_lut = []
    ars_svpwm_lut = []
    nullv0_svpwm_lut = []
    for angle in range(360):
        angles += [angle]

        spwm_lut += [int(round(commutation_techniques.spwm(angle, 100, 100)[0]))]
        ars_svpwm_lut += [int(round(commutation_techniques.ars_svpwm(angle, 100, 100)[0]))]
        nullv0_svpwm_lut += [int(round(commutation_techniques.nullv0_svpwm(angle, 100, 100)[0]))]

    # look up tables
    spwm_lut_var = csnake.Variable('SPWM_LUT', 'uint8_t', value=spwm_lut)
    ars_svpwm_lut_var = csnake.Variable('ARS_SVPWM_LUT', 'uint8_t', value=ars_svpwm_lut)
    null_v0_svpwm_lut_var = csnake.Variable('NULLV0_SVPWM_LUT', 'uint8_t', value=nullv0_svpwm_lut)

    # duty cycle structure
    duty_cycle_struct = csnake.Struct('Duty_Cycle_Struct', typedef=True)
    [duty_cycle_struct.add_variable( (i, 'uint8_t') ) for i in ['a', 'b', 'c']]

    # commutation type
    comm_type_enum = csnake.Enum('commutation_type_enum', typedef=True)
    comm_type_enum.add_values( ['spwm_e', 'ars_svpwm_e', 'nullv0_svpwm_e'] )

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
    cw.start_if_def('COMM_LOOK_UP_TABLE_CH', invert=True)
    cw.add_define('COMM_LOOK_UP_TABLE_CH')
    cw.add_line()
    cw.add_line(comment='Lookup table used for Sinusoidal PWM commutation')
    cw.add_variable_initialization(spwm_lut_var)
    cw.add_line()
    cw.add_line(comment='Lookup table used for Alternating Reversing Sequence Space Vector PWM commutation:')
    cw.add_line(comment='Understanding Space Vector Modulation by Peter Pinewski https://go.gale.com/ps/i.do?id=GALE%7CA18320578&sid=sitemap&v=2.1&it=r&p=AONE&sw=w&userGroupName=anon%7Ebc1f9809')
    cw.add_line(comment='https://youtu.be/5eQyoVMz1dY')
    cw.add_variable_initialization(ars_svpwm_lut_var)
    cw.add_line()
    cw.add_line(comment='Lookup Table used for Null-V0 Space Vector PWM commutation:')
    cw.add_line(comment='Understanding Space Vector Modulation by Peter Pinewski https://go.gale.com/ps/i.do?id=GALE%7CA18320578&sid=sitemap&v=2.1&it=r&p=AONE&sw=w&userGroupName=anon%7Ebc1f9809')
    cw.add_line(comment='https://youtu.be/5eQyoVMz1dY')
    cw.add_variable_initialization(null_v0_svpwm_lut_var)
    cw.add_line()
    cw.end_if_def()
    cw.write_to_file('../include/CommutationLookupTable.ch')


    # recreate each phase for the commutation method using the lookup tables to ensure they are correct
    spwm_phases = []
    ars_svpwm_phases = []
    nullv0_svpwm_phases = []
    for angle in range(360):
        # always 0% field weakening
        a = (angle + 90) % 360
        b = (angle + 90 + 240) % 360
        c = (angle + 90 + 120) % 360

        spwm_phases += [[spwm_lut[a], spwm_lut[b], spwm_lut[c]]]
        ars_svpwm_phases += [[ars_svpwm_lut[a], ars_svpwm_lut[b], ars_svpwm_lut[c]]]
        nullv0_svpwm_phases += [[nullv0_svpwm_lut[a], nullv0_svpwm_lut[b], nullv0_svpwm_lut[c]]]

    # plot the results
    fig, axs = plt.subplots(4)

    axs[0].set_title('Lookup Table Results')
    axs[0].plot(angles, spwm_lut, 'b-', angles, ars_svpwm_lut, 'g-', angles, nullv0_svpwm_lut, 'r-')
    axs[0].legend(['spwm', 'ars svpwm', 'nullv0 svpwm'])
    axs[0].set_xlabel('Direct Axis Angle, deg')
    axs[0].set_ylabel('High Side Transistor Duty Cycle, percent')

    axs[1].set_title('3 phase recreation using the SPWM LUT')
    axs[1].plot(angles, spwm_phases)
    axs[1].legend(['a', 'b', 'c'])
    axs[1].set_xlabel('Direct Axis Angle, deg')
    axs[1].set_ylabel('High Side Transistor Duty Cycle, percent')

    axs[2].set_title('3 phase recreation using the ARS SVPWM LUT')
    axs[2].plot(angles, ars_svpwm_phases)
    axs[2].legend(['a', 'b', 'c'])
    axs[2].set_xlabel('Direct Axis Angle, deg')
    axs[2].set_ylabel('High Side Transistor Duty Cycle, percent')

    axs[3].set_title('3 phase recreation using the NULL=V0 SVPWM LUT')
    axs[3].plot(angles, nullv0_svpwm_phases)
    axs[3].legend(['a', 'b', 'c'])
    axs[3].set_xlabel('Direct Axis Angle, deg')
    axs[3].set_ylabel('High Side Transistor Duty Cycle, percent')

    plt.show()