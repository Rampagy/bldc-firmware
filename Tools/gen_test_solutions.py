import commutation_techniques


def gen_spwm_3_phases(angle, throttle, field_weakening):
    a, b, c = [int(round(phase)) for phase in commutation_techniques.spwm(angle, throttle, field_weakening)]
    return (angle, throttle, field_weakening, a, b, c)

def gen_ars_svpwm_3_phases(angle, throttle, field_weakening):
    a, b, c = [int(round(phase)) for phase in commutation_techniques.ars_svpwm(angle, throttle, field_weakening)]
    return (angle, throttle, field_weakening, a, b, c)

def gen_nullv0_svpwm_3_phases(angle, throttle, field_weakening):
    a, b, c = [int(round(phase)) for phase in commutation_techniques.nullv0_svpwm(angle, throttle, field_weakening)]
    return (angle, throttle, field_weakening, a, b, c)


def save_solutions(name, dutys):
    # generate solutions to test against
    with open(name, 'w') as f:
        for angle, throttle, field_weakening, a, b, c in dutys:
            f.write('{:d},{:d},{:d},{:d},{:d},{:d},\n'.format(angle, throttle, field_weakening, a, b, c))

if __name__ == '__main__':
    spwm_phases = []
    ars_svpwm_phases = []
    nullv0_svpwm_phases = []
    for throttle in [100, 50, 0, -50, -100]:
        for field_weakening in [0, 50, 100]:
            for angle in range(360):
                spwm_phases += [gen_spwm_3_phases(angle, throttle, field_weakening)]
                ars_svpwm_phases += [gen_ars_svpwm_3_phases(angle, throttle, field_weakening)]
                nullv0_svpwm_phases += [gen_nullv0_svpwm_3_phases(angle, throttle, field_weakening)]


    save_solutions('../tests/solutions/spwm.csv', spwm_phases)
    save_solutions('../tests/solutions/ars_svpwm.csv', ars_svpwm_phases)
    save_solutions('../tests/solutions/nullv0_svpwm.csv', nullv0_svpwm_phases)