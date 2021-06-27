/**
 * BIN - cellular automata - majority problem
 * 
 * @author Alena Tesařová (xtesar36)
 * @date 16.4.2021
 **/

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstring>

/***
 * Class for simulation of of celular automata with specific rule and configuration
 */
class CAsim
{
public:
	CAsim(int length, int neighborhood, int steps_max) : length_(length), neighborhood_(neighborhood), steps_max_(steps_max)
	{
		states_mem_ = new int[steps_max_ * length_];
		rules_length_ = RULES_LENGTH;
		rules_ = new int[RULES_LENGTH];
	}

	~CAsim()
	{
		delete[] states_mem_;
		delete[] rules_;
	}

	/** 
	 * @brief Set initial simulation state
	 * @param rules behavior of cellular automata (rules)
	 * @param config configuration
	 **/
	void set_init(int *rules, int *config)
	{
		memcpy(rules_, rules, sizeof(int) * rules_length_);
		memcpy(states_mem_, config, sizeof(int) * length_);
	}

	/***
     * @brief Returns 1D array of length length_
	 * @param step current step of simulation
     **/
	int *get_states(int step)
	{
		assert(step >= 1 && step < steps_max_); // nebudeme cist nulty stav
		return states_mem_ + (step * length_);
	}

	/***
	 * @brief Run simulation for specific amount of steps
	 * @param steps number of steps
	 */
	void run_sim(int steps)
	{
		assert(steps <= steps_max_);

		for (int i = 1; i < steps; i++)
		{
			int *src = states_mem_ + ((i - 1) * length_);
			int *dest = states_mem_ + (i * length_);

			for (int r = 0; r < length_; r++)
			{
				applyRules(src, dest, r);
			}
		}
	}

private:
	int length_;	   // length of configuration
	int rules_length_; // rules length that depends on the neighborhood
	int *rules_;	   // 1d array with rules
	int neighborhood_; // number of cells from left and right of the main cell that will be computed with current cell
	int *states_mem_;  // all states/configurations of one simulation
	int steps_max_;	   // maximum number of steps

	/**
	 * @brief Apply rule to specific element 
	 * @param cells 1D int array of current cells
	 * @param nextCells 1D int array of next cells (after rules are applied)
	 * @param elemIndex index if array that will be computed depending all on its own state and state of its neighbors
	 **/
	void applyRules(int *cells, int *nextCells, int elemIndex)
	{
		int ruleIndex = findRuleIndex(cells, elemIndex);
		// std::cout << "elemIndex is: " << elemIndex << ", ruleIndex is: " << findRuleIndex(cells, elemIndex) << ", result: " << rules_[ruleIndex] << std::endl;
		nextCells[elemIndex] = rules_[ruleIndex];
	}

	/**
	 * @brief Control borders
	 * @param cells 1D int array of current cellular automata
	 * @param index elemtn index for which we search for the rule
	 **/
	int cellValue(int *grid, int index)
	{
		if (index < 0 || index >= length_)
		{
			return 0;
		}
		return grid[index];
	} // end of cellValue
	/**
	 * @brief Find rule index that should be apply to specific index of celullar automata
	 * @param cells 1D int array of current cellular automata
	 * @param index elemtn index for which we search for the rule
	 **/
	int findRuleIndex(int *cells, int index)
	{
		int ruleIndex = 0;													 // calculated rule index as sum of pows of 2
		int j = 1;															 // for indexing ruleIndex
		for (int i = index + neighborhood_; i >= index - neighborhood_; i--) // for throw all the neighbors
		{
			ruleIndex += cellValue(cells, i) * j;
			j = j + j; // 1, 2, 4, 8, 16 ...
		}

		return ruleIndex;
	} // end of findRuleIndex
};