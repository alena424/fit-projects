'''
PBI - Pokročilá bioinformatika
Projekt
Alena Tesařová (xtesar36)
@date 25.12.2020

Zadání c. 2:
2) Implementujte vlastní dokovací algoritmus, který bude vizualizovat průběh přibližování dvou molekul
(jeden uživatelem zvolený protein a jedna libovolná molekula) v Pymolu za účelem nalezení vzájemné polohy
s maximálním počtem kontaktů (pár atomů A,B kde každý atom je z jiné molekuly a vzdálenost |AB| < 5 angstromů) a
minimální střední vzdálenosti sousedících atomů těchto dvou molekul.

Poměr vah těhto dvou kritérií ať je v poměru velikosti molekul (čím větší protein, tím důležitější jsou kontakty).
Není nutné, aby Váš algoritmus našel globálně optimální řešení, musí ale pokud to jde dokovací kriterium zlepšovat a
vypisovat jeho klesající hodnotu na obrazovce.
'''

from random import randint
from pymol import cmd
import numpy
from pymol import stored

import pairwisedistances

# logging
LOG_ENABLE = False
def log(mess):
    if LOG_ENABLE:
        print(mess)


# Number of angstorms that is counted as a contact
CONTACT_LIMIT = 5

# Number of angstorms that is counted is a neighbor
NEIGHBOR_LIMIT = 15

# Deault depth of algorithm
ALGORITHM_DEPTH = 2

# Depending on chosen molecule
MAX_NUM_CONTACT = 10

# depending of the size of protein
score_ratio_contact = 10

# Set global score
global_score = 0

# Name of molecule
molecule_name = "GLY"

# Name of protein 
protein_name = "protein_sel"

label_name = "label_score"

num_iteration = 1
    
# The best position of the molecule with the highest score
best_position = 0

# Init all possible moves
moves = [[]] * 27

def init_globals():
    """
    Init global variable while starting the algorithm
    """
    global global_score
    global num_iteration
    global_score = 0
    num_iteration = 1


# All 28 directions
def init_moves(step):
    """
    Init length of one step (could be changed during algorithm)
    """
    global moves
    moves[0] = [0, 0, 0]
    moves[1] = [step, 0, 0]
    moves[2] = [step, step, 0]
    moves[3] = [step, 0, step]
    moves[4] = [0, step, 0]
    moves[5] = [0, 0, step]
    moves[6] = [0, step, step]
    moves[7] = [step, step, step]
    moves[8]  = [-step, -step, -step]
    moves[9]  = [-step, 0, 0]
    moves[10] = [-step, -step, 0]
    moves[11] = [-step, 0, -step]
    moves[12] = [0, -step, 0]
    moves[13] = [0, 0, -step]
    moves[14] = [0, -step, -step]
    moves[15] = [-step, step, -step]
    moves[16] = [-step, -step, step]
    moves[17] = [step, -step, -step]
    moves[18] = [step, step, -step]
    moves[19] = [-step, step, step]
    moves[20] = [step, -step, step]
    moves[21] = [-step, step, 0]
    moves[22] = [step, -step, 0]
    moves[23] = [step, 0, -step]
    moves[24] = [-step, 0, step]
    moves[25] = [0, -step, step]
    moves[26] = [0, step, -step]


ratio_number_contact = 100
# Default bonus is set to 1
protein_bonus = 1

MIN_PROTEIN_LENGTH = 2000
MAX_PROTEIN_LENGTH = 8000
SCALE_LENGTH = MAX_PROTEIN_LENGTH - MIN_PROTEIN_LENGTH

def count_score(number_of_contact, average_distance):
    """
    Function counts the total score of algorithm
    - number_of_contact must be the highest
    - average_distance the lowest
    """
    return (number_of_contact * protein_bonus)/ratio_number_contact + (1/average_distance)


def protein_length():
    """
    Return the number of atoms of protein
    """
    obj_list = cmd.get_model(protein_name)
    return len(obj_list.atom)

def calculate_protein_bonus():
    """
    Calculates bonus for large protein
    """
    global protein_bonus
    protein_len = protein_length()
    if (protein_len < MIN_PROTEIN_LENGTH):
        # Set minimum ration to 1
        protein_bonus = 1
        return
    if protein_len > MAX_PROTEIN_LENGTH:
        # Set maximum ration to 1
        protein_bonus = 2
        return
    protein_bonus = 1 + (protein_len - MIN_PROTEIN_LENGTH)/SCALE_LENGTH
    return


def find_solution(show="N", output="N", level=0, alg_depth=ALGORITHM_DEPTH, step=1):
    """
    Main Function that finds the best location of molecule
    Princip:
        Molucule moves to 28 directions and tries to improve the score
        If score was improved, takes this direction and repeats this step
        Meanwhile each nonh successfull move can have another "try" -
         - takes the location and moves again (the depth of DFS is set to ALGORITHM_DEPTH)
         - trying to beat the best score
    Params:
        level - current level for DFS
        alg_depth - the limitation for DFS (tested with values from 2-8)
        show - shows (Y) individual distances in pymol menu (default=N)
        output - output of pairwise (viz pairwise), accepts Screen/Print/None (default N)
    """
    global global_score
    global num_iteration
    global best_position

    improved = False
    default_xyz = cmd.get_coords(molecule_name, 1)
    # Init number of each step of move
    init_moves(step=step)
    for move in moves:
        current_score = 0.0
        cmd.translate(move, molecule_name)
        
        # Get number of contacts 
        _, contacts, is_overlapping = pairwisedistances.pairwise_dist(sel1=protein_name, sel2=molecule_name, max_dist=CONTACT_LIMIT,
                                                      show=show,
                                                      output=output)
        if (is_overlapping):
            # Set back coordinates
            cmd.load_coords(default_xyz, molecule_name)
            continue

        # Get total distance from molecule neighbours and number of neighbors detected - used for counting the score
        total_distance, counter, _ = pairwisedistances.pairwise_dist(sel1=protein_name, sel2=molecule_name,
                                                                  max_dist=NEIGHBOR_LIMIT,
                                                                  show="N", output="N")
        # If there is no neighbor, score is zero
        if counter:
            current_score = count_score(contacts, average_distance=total_distance / counter)
            print("Score is: " + str(current_score) + ", contacts: " + str(contacts) + ", neighbors: " + str(counter))
        
        if current_score > global_score:
            # Score improved
            improved = True
            global_score = current_score
            # Remember the best score
            best_position = cmd.get_coords(molecule_name, 1)
            take_screen()

        # No improvement, we try to get further - limitation of iteration is set to ALGORITHM_DEPTH
        else:
            if current_score > 0.1 and level < alg_depth:
                log("Trying this direction " + str(alg_depth) + " level " + str(level))
                find_solution(show, output, level=level+1, alg_depth=alg_depth,step=step+1)

        # Set back coordinates
        cmd.load_coords(default_xyz, molecule_name)

    # Continue this way if there was an improvement
    if improved:
        print("Score is: " + str(current_score) + ", contacts: " + str(contacts) + ", neighbors: " + str(counter))
        num_iteration = num_iteration + 1
        # Load the best position
        cmd.load_coords(best_position, molecule_name)
        take_screen()
        find_solution(show, output, alg_depth=alg_depth,step=step)
       

# Counter for taking screenshots
pic_num = 1
def take_screen(name="pic"):
    """
    Functin takes screenshot of current scene
    """
    global pic_num
    cmd.center()
    cmd.zoom()
    # Work only with mpng
    cmd.mpng("img/test/test") # this img is not important for us
    cmd.png("img/pic" + str(pic_num) + ".png", width=1200, height=800, dpi=-1,ray=0, quiet=0)
    pic_num = pic_num + 1 


def find_protein():
    """
    Function goes along the axis x with molecule to find the protein
    If axis x is negative, it terminates or of there are enought neighbors found
    """
    coords = cmd.get_coords(molecule_name, 1)
    if (coords[0][0] < 0.0):
        log("Out of x: " + str(coords[0]) + str(coords))
        return
    _, counter, _ = pairwisedistances.pairwise_dist(sel1=protein_name, sel2=molecule_name,max_dist=NEIGHBOR_LIMIT-2)
    if counter == 0:
        cmd.translate([-10,0,0], molecule_name)
        take_screen()
        find_protein()


def run(pbi_id="2c0k",step=2, alg_depth=ALGORITHM_DEPTH):
    """
    Main function to run the program and set main parameters
    Method tries to find the best solution for docking protein to molecule
    usage: run([pbi_id, output, show, step, alg_depth])
    Params:
        pbi_id - ID of protein from PDB (it fetches automatically the protein)
        alg_depth - the limitation for DFS (tested with values from 2-8)
    """

    # Fetch protein from PDB
    cmd.fetch(pbi_id, protein_name) 
    cmd.load(molecule_name + ".cif")
    
    # Move molecule far from protein
    cmd.translate([300,0,0], molecule_name)
    
    # Take initial screenshot
    take_screen()

    # Finding the protein
    find_protein()

    # Count the bonus for large protein
    calculate_protein_bonus()

    # Start docking
    find_solution("N","N",alg_depth=alg_depth,step=step)

    print("The best score is: " + str(global_score))
    cmd.load_coords(best_position, molecule_name)
    take_screen()

def test():
    """
    Method to test the algorithm with different step and depth
    Saves result to test/test.txt
    """
    f = open('test/test' +'.txt', 'w')
    for alg_depth in range (2,3):
        for x in range(1,10):
            f.write("-- TEST " + str(x) + "--\n")
            init_globals()
            cmd.delete("*")
            run(step=x, alg_depth=alg_depth)
            _, contacts, is_overlapping = pairwisedistances.pairwise_dist(sel1=protein_name, sel2=molecule_name, max_dist=CONTACT_LIMIT,
                                                          show="N",
                                                          output="N")
            total_distance, counter, _ = pairwisedistances.pairwise_dist(sel1=protein_name, sel2=molecule_name,
                                                                          max_dist=NEIGHBOR_LIMIT,
                                                                          show="N", output="N")
            # If there is no neighbor, score is zero
            if counter:
                current_score = count_score(contacts, average_distance=total_distance / counter)
                f.write("Score is: " + str(current_score) + ", contact: " + str(contacts) + ", distance: " + str(total_distance/counter))

            f.write("\nAlg depth " + str(alg_depth))
            f.write("\nStep " + str(x))
            f.write("\nIteration: " + str(num_iteration) + "\n")
            f.write("------------------------------------------------------\n\n")
            take_screen("pic" + str(x) + str(alg_depth))
    f.close()