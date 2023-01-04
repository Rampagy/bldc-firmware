import commutation_techniques


def gen_spwm_3_phases(angle, throttle, field_weakening):
    a, b, c = [int(round(phase)) for phase in commutation_techniques.spwm(angle, throttle, field_weakening)]
    return (angle, throttle, field_weakening, a, b, c)

def gen_svpwm_3_phases(angle, throttle, field_weakening):
    a, b, c = [int(round(phase)) for phase in commutation_techniques.svpwm(angle, throttle, field_weakening)]
    return (angle, throttle, field_weakening, a, b, c)


if __name__ == '__main__':
    spwm_phases = []
    svpwm_phases = []
    for throttle in [100, 50, 0, -50, -100]:
        for field_weakening in [0, 50, 100]:
            for angle in range(360):
                spwm_phases += [gen_spwm_3_phases(angle, throttle, field_weakening)]
                svpwm_phases += [gen_svpwm_3_phases(angle, throttle, field_weakening)]

    # generate solutions to test against
    with open('../tests/solutions/spwm.csv', 'w') as f:
        for angle, throttle, field_weakening, a, b, c in spwm_phases:
            f.write('{:d},{:d},{:d},{:d},{:d},{:d},\n'.format(angle, throttle, field_weakening, a, b, c))

    # generate solutions to test against
    with open('../tests/solutions/svpwm.csv', 'w') as f:
        for angle, throttle, field_weakening, a, b, c in svpwm_phases:
            f.write('{:d},{:d},{:d},{:d},{:d},{:d},\n'.format(angle, throttle, field_weakening, a, b, c))