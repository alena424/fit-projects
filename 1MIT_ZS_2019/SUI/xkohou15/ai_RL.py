import logging
from keras.models import Sequential
from keras.layers import Dense
from keras.models import model_from_json, load_model
import os
from .turns_collector import *
from ..utils import possible_attacks

from dicewars.client.ai_driver import BattleCommand, EndTurnCommand


class AI:
    """Reinforcement learning
    
    This agent tries to learn how to behave with neural network
    """ 
    def __init__(self, player_name, board, players_order):
        self.player_name = player_name
        self.logger = logging.getLogger('AI_RL')
        self.logger.setLevel(logging.CRITICAL)
        self.loaded_model = load_model("dicewars/ai/xkohou15/default_model.h5")
        self.attack_rate_array = [0.45,0.40,0.45,0.48,0.53,0.55,0.58]

    def ai_turn(self, board, nb_moves_this_turn, nb_turns_this_game, time_left):
        # load json and create model

        attacks = list(possible_attacks(board, self.player_name))
        scores = []
        nb_players = board.nb_players_alive()

        for source, target in attacks:
            turn = Turn(board, self.player_name, source, target)
            turn_info = turn.get_vector_for_learning()
            turn_info_arr = [turn_info]
            score = self.loaded_model.predict(numpy.array(turn_info_arr))[0]
            scores.append([source, target, score, score[0]])

        if len(scores) > 0:
            scores = sorted(scores, key=lambda turn: turn[3], reverse=True)
            score = scores[0][2]
            source = scores[0][0]
            target = scores[0][1]
            self.logger.debug("%.2f: %.2f" % (score[0], score[1]))
            attack_rate = self.attack_rate_array[nb_players-2]
            dice_rate = self.dice_attack_rate(board)
            if score[0] >= (attack_rate+dice_rate) or source.get_dice() == 8:  # this turn leads to win
                return BattleCommand(source.get_name(), target.get_name())

        return EndTurnCommand()

    def dice_attack_rate(self, board):
        dice_array = [0.08,0.05,0.03, -0.055, -0.060, -0.12]
        index = int(board.get_player_dice(self.player_name) / 10)
        index = (index if index < len(dice_array) else len(dice_array)-1)
        return dice_array[index]
