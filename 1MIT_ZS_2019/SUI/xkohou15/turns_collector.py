import pickle, sys, os, numpy


from enum import Enum

from dicewars.client.game.board import Board
from dicewars.client.game.area import Area
from dicewars.ai.utils import probability_of_holding_area, probability_of_successful_attack

file_name = "dicewars/ai/xkohou15/temporary_turns"


class TurnsCollector:

    def __init__(self):
        self.pickle_file = open(file_name, 'wb')
        print("TurnCollector created file " + file_name)
        self.last_turn = []
        self.store_to_file = False
        if self.store_to_file:
            csv_file = file_name + "_to_corelate.csv"
            if not os.path.exists(csv_file):
                f = open(csv_file, "w")
                f.close()

            self.store_file = open(csv_file, 'a')
            if os.stat(csv_file).st_size == 0:
                self.store_file.write(column_names() + "\n")
        else:
            self.store_file = None

    def __del__(self):
        self.pickle_file.close()

    def store_turn(self, turn):
        """
        call this to set up Turn to store in next turn
        """
        self.last_turn.append(turn)

    def actualize_last_turn(self, board):
        """
        this is stores turn to file
        """
        if len(self.last_turn) == 0:
            return

        #turn = self.last_turn

        for turn in self.last_turn:
            if board.get_area(turn.target.get_name()).get_owner_name() == turn.player_name:
                turn.holded()
            else:
                turn.not_holded()

            pickle.dump(turn, self.pickle_file)
            if self.store_file is not None:
                self.store_file.write(turn.analyzation_string() + "\n")

        self.last_turn = []


class TurnsLoader:

    def __init__(self, file=file_name):
        self.pickle_file = open(file, 'rb')

    def __del__(self):
        self.pickle_file.close()

    def load_turn(self):
        return pickle.load(self.pickle_file)


class Turn:

    class GAME_RESULT(Enum):
        WIN = 1
        LOST = 0
        NOT_HANDLED = 3

    class TURN_RESULT(Enum):
        HELD = 1
        NOT_HELD = 0
        NOT_HANDLED = 3

    def __init__(self, board, player_name, source, target, result=GAME_RESULT.NOT_HANDLED, held=TURN_RESULT.NOT_HANDLED):
        self.board = board
        self.player_name = player_name
        self.nb_of_players = self.board.nb_players_alive()

        self.source = source
        self.source_dice = source.get_dice()

        self.target = target
        self.target_dice = target.get_dice()

        self.att_prob = probability_of_successful_attack(self.board, self.source.get_name(), self.target.get_name())
        self.hold_prob = probability_of_holding_area(self.board, self.target.get_name(), self.source_dice - 1, self.player_name)

        self.allied_nb_neigh = len(self.get_neigh())
        self.enemy_nb_neigh = len(self.get_neigh(allied=False))

        self.allied_dice_neigh = self.get_num_of_neigh_dices()
        self.enemy_dice_neigh = self.get_num_of_neigh_dices(allied=False)

        self.potencial_score_growth = 0
        self.set_other_attributes()
        self.from_largest_area = (1 if source.get_name() in self.largest_region else 0)

        self.largest_region = []
        self.held = held
        self.result = result

    def get_num_of_neigh_dices(self, allied=True):
        num_of_dices = 0

        for area in self.get_neigh(allied):
            num_of_dices += self.board.get_area(area).get_dice()

        return num_of_dices

    def get_neigh(self, allied=True):
        list_of_neigh = []
        if allied:
            for area in self.target.get_adjacent_areas():
                if self.board.get_area(area).get_owner_name() == self.source.get_owner_name():
                    list_of_neigh.append(area)
        else:
            for area in self.target.get_adjacent_areas():
                if self.board.get_area(area).get_owner_name() != self.source.get_owner_name():
                    list_of_neigh.append(area)

        return list_of_neigh

    def set_other_attributes(self):
        """
        Count other attributes to store
            attributes to store:
                source.name
                source.dice
                target.name
                target.dice
                result

        """
        actual_size = self.get_largest_region()

        area = self.board.get_area(self.target.get_name())
        previous_dices = area.get_dice()
        area.set_dice(self.source_dice-1)
        previous_owner = area.owner_name
        area.set_owner(self.player_name)

        new_size = self.get_largest_region()

        """UNDO action"""
        area.set_owner(previous_owner)
        area.set_dice(previous_dices)
        self.get_largest_region()
        """unsuccessful attack END"""

        self.potencial_score_growth = new_size - actual_size
        #print("potencial growth " + str(self.potencial_score_growth))

    def set_result(self, result):
        self.result = result

    def not_holded(self):
        self.held = Turn.TURN_RESULT.NOT_HELD

    def holded(self):
        self.held = Turn.TURN_RESULT.HELD

    def get_largest_region(self):
        """
        Got from wpm_c.py
        """
        self.largest_region = []

        players_regions = self.board.get_players_regions(self.player_name)
        max_region_size = max(len(region) for region in players_regions)
        max_sized_regions = [region for region in players_regions if len(region) == max_region_size]

        for region in max_sized_regions:
            for area in region:
                self.largest_region.append(area)
        return max_region_size

    def get_vector_for_learning(self):
        return numpy.array([
                self.source_dice,
                self.target_dice,
                self.att_prob,
                self.nb_of_players,
                self.hold_prob,
                self.allied_nb_neigh,
                self.enemy_nb_neigh,
                self.allied_dice_neigh,
                self.enemy_dice_neigh,
                self.potencial_score_growth,
                self.from_largest_area
                ])

    def analyzation_string(self):
        return ';'.join([
            str(self.source_dice),
            str(self.target_dice),
            str(self.att_prob),
            str(self.nb_of_players),
            str(self.hold_prob),
            str(self.allied_nb_neigh),
            str(self.enemy_nb_neigh),
            str(self.allied_dice_neigh),
            str(self.enemy_dice_neigh),
            str(self.potencial_score_growth),
            str(self.from_largest_area),
            str(self.held),
            str(self.win)
        ])

    def get_vector_result_for_learning(self):
        if self.result == Turn.GAME_RESULT.WIN:
            return numpy.array([1, 0])
        else:
            return numpy.array([0, 1])

    def get_vector_holded_for_learning(self):
        if self.held == Turn.TURN_RESULT.HELD:
            return numpy.array([1, 0])
        else:
            return numpy.array([0, 1])

def column_names():
    return ';'.join([
        "source_dice",
        "target_dice",
        "att_prob",
        "nb_of_players",
        "hold_prob",
        "allied_nb_neigh",
        "enemy_nb_neigh",
        "allied_dice_neigh",
        "enemy_dice_neigh",
        "potencial_score_growth",
        "from_largest_area",
        "held",
        "win"
    ])