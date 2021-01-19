'''
PBI - Pokročilá bioinformatika
Domácí úkol č. 2
Alena Tesařová (xtesar36)
@date 25.11.2020

Zadání:
Vytvořte tripeptid Ala-Gly-Ala a napište skript v Pythonu (využívající
Pymol API, sparametry t, p a m) pro náhodne otáčení krajních aminokyselin kolem střední
s počtem otočení daným parametrem t. Po každých p otočeních vypište vzdálenost C-alfa uhlíků
dvou krajních alaninů. Odhadněte minimální a maximální možnou vzdálenost ve dvou
verzích programu, jedné která nekontroluje kolizi a druhá, která nedovoluje otočení
v případech vedoucích ke kolizi (posuzovanou jako vzdálenost mezi atomy menší než m).
'''

from random import randint
from pymol import cmd

# logging
LOG_ENABLE = False

# array of all counted distances
distances = []
# total number of collisions
num_collisions = 0


def log(mess):
    if LOG_ENABLE:
        print(mess)

log('Loaded')

def run(m=0, control_collision=False):
    log('Started')
    global num_collisions
    m = int(m)
    # build alanin
    cmd.select('alanin1', '/triplet///ALA`2/')
    cmd.select('alanin2', '/triplet///ALA`4/')
    cmd.select('alanin1_CA', '/triplet///ALA`2/CA')
    cmd.select('alanin2_CA', '/triplet///ALA`4/CA')

    angle1 = randint(0, 360)
    x1 = randint(0, 1)
    y1 = randint(0, 1)
    z1 = randint(0, 1)
    log('Rotating: ' + str(angle1))
    cmd.rotate([x1, y1, z1], angle1, 'alanin1')

    angle2 = randint(0, 360)
    x2 = randint(0, 1)
    y2 = randint(0, 1)
    z2 = randint(0, 1)
    log('Rotating: ' + str(angle2))
    cmd.rotate([x2, y2, z2], angle2, 'alanin2')

    distance = cmd.distance("distance_alanin", "alanin1_CA", "alanin2_CA")

    if control_collision:
        if distance < m:
            log('Collision detected, rotating back' + str(num_collisions))
            cmd.rotate([x1, y1, z1], -angle1, 'alanin1')
            cmd.rotate([x2, y2, z2], -angle2, 'alanin2')
            num_collisions = num_collisions + 1
        else:
            distances.append(distance)
    else:
        # no controll
        distances.append(distance)

    return distance


def runAll(t=10, p=1, m=0, control_collision=False):
    log('Started')
    cmd.load('triplet.cif')
    t, p, m = int(t), int(p), int(m)
    counter = 1
    var_counter = 1
    if t < 0 or p < 0:
        print("Parameter can not be lower than zero")
        return
    while counter != t:
        curr_distance = run(m, control_collision)
        if var_counter == p:
            print(counter, ": Distance is: ", curr_distance)
            var_counter = 0
        counter = counter + 1
        var_counter = var_counter + 1

    print("Minimum is: ", min(distances))
    print("Maximum is: ", max(distances))
    print("Average is: ", sum(distances) / len(distances))
    if control_collision:
        print("Number of collisions: ", num_collisions)
