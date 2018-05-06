# ITS
# 1. project based on BDD testing
# @date		28.03.2018
# @author	Alena Tesarova, xtesar36

# Shopping process 1/3

Feature: Choosing item

	# no products in category
	Scenario: Display empty category
		Given shopping web page Opencart is displayed
		When the user clicks on Software
		Then the user sees There are no products to list in this category

	# user wants to find what he wants to buy
 	Scenario Outline: Display all cameras
		Given shopping web page Opencart is displayed
		When the user clicks on Cameras in Categories
		Then the user sees <cameras>
        Examples: Cameras
		| cameras     |
		| Canon EOS 5D|
		| Nikon D300  |

	Scenario Outline: Display category Laptops & Notebooks
		Given shopping web page Opencart is displayed
		When the user clicks on Laptops & Notebooks
		Then the user sees dropmenu containing <labtopsnotebooks>
        Examples: labtopsnotebooks
		| labtopsnotebooks |
		| Macs             |
		| Windows          |

########### end of tests ################
