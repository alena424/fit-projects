import logging
from random import shuffle

from ..utils import possible_attacks

from dicewars.client.ai_driver import BattleCommand, EndTurnCommand
from .turns_collector import *

class AI:
    """Naive player agent

    This agent performs all possible moves in random order
    """

    def __init__(self, player_name, board, players_order):
        """
        Parameters
        ----------
        game : Game
        """
        self.player_name = player_name
        self.logger = logging.getLogger('AI')
        self.collection = TurnsCollector()
        self.first_move_of_turn = False # we don+t want to check it on start


    def ai_turn(self, board, nb_moves_this_turn, nb_turns_this_game, time_left):
        """AI agent's turn

        Get a random area. If it has a possible move, the agent will do it.
        If there are no more moves, the agent ends its turn.
        """
        if self.first_move_of_turn:
            self.collection.actualize_last_turn(board)

        self.first_move_of_turn = False
        attacks = list(possible_attacks(board, self.player_name))
        shuffle(attacks)
        for source, target in attacks:
            "Get information to be stored"
            turn = Turn(board, self.player_name, source, target)
            self.collection.store_turn(turn)
            return BattleCommand(source.get_name(), target.get_name())


        self.logger.debug("No more possible turns.")
        self.first_move_of_turn = True
        return EndTurnCommand()
