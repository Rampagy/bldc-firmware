import math
import sys


def degToRad(angDeg):
    return math.radians(angDeg)

def sineLookup(angle):
    return 0.5*math.cos(degToRad(angle))+0.5

def trapezoidal(direct_axis):
    # returns tuple of each FET duty cycle (Q1 duty, Q2 duty, Q3 duty, Q4 duty, Q5 duty, Q6 duty)

    sector = direct_axis // 60
    if sector == 0: # 0b101
        # 0 to 60 degrees
        return (100, 0, 0, 100, 0, 0)
    elif sector == 1: # 0b100
        # 60 to 120 degrees
        return (100, 0, 0, 0, 100, 0)
    elif sector == 2: # 0b110
        # 120 to 180 degrees
        return (0, 0, 100, 0, 0, 100)
    elif sector == 3: # 0b010
        # 180 to 240 degrees
        return (0, 100, 100, 0, 0, 0)
    elif sector == 4: # 0b011
        # 240 to 300 degrees
        return (0, 100, 0, 0, 100, 0)
    elif sector == 5: # 0b001
        # 300 to 360 degrees
        return (0, 0, 0, 100, 100, 0)

def spwm(direct_axis, throttle, field_weakening):
    # throttle (-100 to 100) is the max duty cycle that will be commanded
    # field weakening (0 to 100) is the percent of field weaking you desire
    # returns tuple of each phases high side mosfet duty cycle (phase a duty, phase b duty, phase c duty)
    # sinsoidal pwm

    # control to 90 degrees ahead of the direct axis
    # to go in reverse swap +90 with -90
    # to field weaken change +/-90 to something closer to 0
    sign = -1 if throttle < 0 else 1
    quadrature_axis = direct_axis + sign*90*(100-field_weakening)/100
    return (  abs(throttle)*sineLookup(quadrature_axis), abs(throttle)*sineLookup(quadrature_axis+240), abs(throttle)*sineLookup(quadrature_axis+120) )

def ars_svpwm(direct_axis, throttle, field_weakening):
    # throttle (-100 to 100) is the max duty cycle that will be commanded
    # field weakening (0 to 100) is the percent of field weaking you desire
    # returns tuple of each phase's duty cycle (phase a duty, phase b duty, phase c duty)
    # Alternating Reversing Sequence Space Vector Pulse Width Modulation
    
    pwm_period = abs(throttle) # 100 #0.00002 # in seconds # this is supposed to be in seconds, but by putting 100 seconds it outputs duty cycle in percent
    duty_cycle = 1 # in percent
    sign = -1 if throttle < 0 else 1

    # control to 90 degrees ahead of the direct axis
    # to go in reverse swap +90 with -90
    # to field weaken change +/-90 to something closer to 0
    quadrature_axis = direct_axis + sign*90*(100-field_weakening)/100

    # this -30 is bugging me, but idk how to get rid of it
    alpha = quadrature_axis % 60
    T1 = pwm_period * duty_cycle * math.sin( degToRad(60-alpha) )
    T2 = pwm_period * duty_cycle * math.sin( degToRad(alpha) )
    T0 = pwm_period - T1 - T2

    #w = 0.5*T0 + T2
    #x = 0.5*T0
    #y = 0.5*T0 + T1
    #z = 0.5*T0 + T1 + T2

    # this -30 is bugging me, but idk how to get rid of it
    sector = (quadrature_axis // 60) % 6
    if sector == 0: # sector 1
        # 0 to 60 degrees
        return ( T1+T2+0.5*T0, T2+0.5*T0, 0.5*T0 )
    elif sector == 1: # sector 2
        # 60 to 120 degrees
        return ( T1 + 0.5*T0, T1+T2+0.5*T0, 0.5*T0 )
    elif sector == 2: # sector 3
        # 120 to 180 degrees
        return ( 0.5*T0, T1+T2+0.5*T0, T2+0.5*T0 )
    elif sector == 3: # sector 4
        # 180 to 240 degrees
        return ( 0.5*T0, T1+0.5*T0,  T1+T2+0.5*T0 )
    elif sector == 4: # sector 5
        # 240 to 300 degrees
        return ( T2+0.5*T0, 0.5*T0,  T1+T2+0.5*T0 )
    elif sector == 5: # sector 6
        # 300 to 360 degrees
        return ( T1+T2+0.5*T0, 0.5*T0, T1+0.5*T0 )


if __name__ == '__main__':
    import matplotlib.pyplot as plt # only needed if this is the main script being run

    trap_dutys = []
    spwm_dutys = []
    svpwm_dutys = []
    electrical_phase = []

    # electrical angle is the direct axis angle
    throttle = 100 # simulate a start from zero throttle ramp
    field_weakening = 0 # simulate changing field weakening
    for electrical_angle in range(1080):
        electrical_phase += [electrical_angle]

        trap_dutys += [trapezoidal(electrical_angle)]
        spwm_dutys += [spwm(electrical_angle, throttle, field_weakening)]
        svpwm_dutys += [ars_svpwm(electrical_angle, throttle, field_weakening)]

        if throttle < 100:
            throttle += 0.15

        if field_weakening > 0:
            field_weakening -= 1


    spwm_a = []
    spwm_b = []
    spwm_c = []
    for a1, b1, c1 in spwm_dutys:
        spwm_a += [a1]
        spwm_b += [b1]
        spwm_c += [c1]

    svpwm_a = []
    svpwm_b = []
    svpwm_c = []
    for a1, b1, c1 in svpwm_dutys:
        svpwm_a += [a1]
        svpwm_b += [b1]
        svpwm_c += [c1]

    # determine the delta voltage (as a percent of VBatt) for each commutation type
    spwm_dV = [max(a, b, c) - min(a, b, c) for a, b, c in spwm_dutys]
    svpwm_dV = [max(a, b, c) - min(a, b, c) for a, b, c in svpwm_dutys]

    fig, axs = plt.subplots(2)
    fig.suptitle('Commutation techniques')

    axs[0].plot(electrical_phase, spwm_a, 'g-', electrical_phase, spwm_b, 'b-', electrical_phase, spwm_c, 'r-',
                electrical_phase, svpwm_a, 'g--', electrical_phase, svpwm_b, 'b--', electrical_phase, svpwm_c, 'r--',)
    axs[0].legend(['SPWM Phase A', 'SPWM Phase B', 'SPWM Phase C',
                    'ARS SVPWM Phase A', 'ARS SVPWM Phase B', 'ARS SVPWM Phase C'])
    axs[0].set_ylabel('High side transistor duty cycle (percent)')
    axs[0].set_xlabel('Direct axis angle (deg)')
    axs[0].set_title('Duty cycle for different commutation techniques')

    axs[1].plot(electrical_phase, spwm_dV, electrical_phase, svpwm_dV)
    axs[1].legend(['SPWM', 'ARS_SVPWM'])
    axs[1].set_ylabel('Phase to phase voltage (percent of battery voltage)')
    axs[1].set_xlabel('Direct axis angle (deg)')
    axs[1].set_title('Phase voltage for different commutation techniques')

    plt.show()
