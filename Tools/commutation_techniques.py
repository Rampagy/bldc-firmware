import math
import matplotlib.pyplot as plt


def degToRad(angDeg):
    return angDeg * math.pi / 180

def sineLookup(angle):
    return 0.5*math.sin(degToRad(angle))+0.5

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

def spwm(direct_axis):
    # returns tuple of each phases high side mosfet duty cycle (phase a duty, phase b duty, phase c duty)
    # sinsoidal pwm
    
    # control to 90 degrees ahead of the direct axis
    # to go in reverse swap +90 with -90
    # to field weaken change +/-90 to something closer to 0
    quadrature_axis = direct_axis + 90
    return (  100*sineLookup(quadrature_axis), 100*sineLookup(quadrature_axis+240), 100*sineLookup(quadrature_axis+120) )

def svpwm(direct_axis):
    # returns tuple of each phase's duty cycle (phase a duty, phase b duty, phase c duty)
    # Alternating Reverse Space Vector Modulation
    
    pwm_period = 100 #0.00002 # in seconds # this is supposed to be in seconds, but by putting 100 seconds it outputs duty cycle in percent
    duty_cycle = 1 # in percent


    # control to 90 degrees ahead of the direct axis
    # to go in reverse swap +90 with -90
    # to field weaken change +/-90 to something closer to 0
    quadrature_axis = direct_axis + 90

    # this -30 is bugging me, but idk how to get rid of it
    alpha = (quadrature_axis-30) % 60
    T1 = pwm_period * duty_cycle * math.sin( degToRad(60-alpha) )
    T2 = pwm_period * duty_cycle * math.sin( degToRad(alpha) )
    T0 = pwm_period - T1 - T2

    w = 0.5*T0 + T2
    x = 0.5*T0
    y = 0.5*T0 + T1
    z = 0.5*T0 + T1 + T2

    # this -30 is bugging me, but idk how to get rid of it
    sector = ((quadrature_axis-30) // 60) % 6
    if sector == 4: # 0b101
        # 0 to 60 degrees
        return ( x, y, z )
    elif sector == 5: # 0b100
        # 60 to 120 degrees
        return ( w, x, z )
    elif sector == 0: # 0b110
        # 120 to 180 degrees
        return ( z, x, y )
    elif sector == 1: # 0b010
        # 180 to 240 degrees
        return ( z, w, x )
    elif sector == 2: # 0b011
        # 240 to 300 degrees
        return ( y, z, x )
    elif sector == 3: # 0b001
        # 300 to 360 degrees
        return ( x, z, w )


if __name__ == '__main__':

    trap_dutys = []
    spwm_dutys = []
    svpwm_dutys = []
    electrical_phase = []

    # elecitrcal angle is the q axis angle
    # assume 100% command...
    for electrical_angle in range(360):
        electrical_phase += [electrical_angle]

        trap_dutys += [trapezoidal(electrical_angle)]
        spwm_dutys += [spwm(electrical_angle)]
        svpwm_dutys += [svpwm(electrical_angle)]


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


    fig, axs = plt.subplots(2)
    fig.suptitle('Commutation techniques')

    axs[0].plot(electrical_phase, spwm_a, electrical_phase, spwm_b, electrical_phase, spwm_c)
    axs[0].legend(['phase a', 'phase b', 'phase c'])
    axs[0].set_ylabel('High Side Transistor Duty Cycle, percent')
    axs[0].set_xlabel('Direct Axis Angle, deg')
    axs[0].set_title('Sinusoidal PWM')

    axs[1].plot(electrical_phase, svpwm_a, electrical_phase, svpwm_b, electrical_phase, svpwm_c)
    axs[1].legend(['phase a', 'phase b', 'phase c'])
    axs[1].set_ylabel('High Side Transistor Duty Cycle, percent')
    axs[1].set_xlabel('Direct Axis Angle, deg')
    axs[1].set_title('Space Vector PWM')

    plt.show()
