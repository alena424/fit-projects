# ITS
# 1. project based on BDD testing
# @date		28.03.2018
# @author	Alena Tesarova, xtesar36 

# Shopping process 2/3

Feature: Shopping cart
  
	# shopping cart empty
	Scenario: Display shopping cart with no product
		Given no product in shopping cart
		When the user clicks on "Shopping cart"
		Then the user sees that shopping cart is empty
	
	# user wants to add item to cart
	Scenario: Adding Nikon to cart
		Given description of camera "Nikon D300" is shown
		When the user types quantity 1
		And the user clicks on "Add to Cart" button
		Then Nikon D300 is added to cart
	
	# shopping cart with one product
	Scenario: Display shopping cart
		Given one available Nikon D300 in shopping cart
		When the user clicks on "Shopping cart"
		Then the user sees Nikon D300 in shopping cart
		But quantity equals one	

	# update quantity in cart
	Scenario: Increase quantity in cart
		Given Nikon D300 in shopping cart
		When the user updates quantity of Nikon D300 to "4"
		Then quantity of Nikon in cart is 4
	
	# remove item from cart
	Scenario: remove Nikon D300 from the cart
		Given Nikon D300 is in shopping cart
		When the user removes Nikon D300
		Then Nikon D300 is no longer in the cart
	
	# not availabe product in desired quantity
	Scenario: Product is not available
		Given Samsung Galaxy Tab 10.1 in shopping cart with 3 stars
		When the user opens shopping cart
		Then the users sees warning

	# apply invalid gift certificate
	Scenario: Enter a invalid gift certificate
		Given Nikon D300 in shopping cart
		And certificate "5678" is invalid
		When the user applies certificate number "5678" 
		Then the user gets warning

	# apply valid gift certificate
	Scenario: Enter a valid gift certificate
		Given Nikon D300 in shopping cart
		And certificate "1234" is valid
		When the user applies certificate number "1234" 
		Then the user sees that discount has been applied

	# apply invalid coupon
	Scenario: Enter a invalid coupon
		Given Nikon D300 in shopping cart
		And coupon 1234 is invalid
		When the user applies coupon number "1234" 
		Then the user gets warning

	# apply valid coupon
	Scenario: Enter a valid coupon
		Given Nikon D300 in shopping cart
		And coupon 1111 is valid
		When the user applies coupon number "1111" 
		Then the user sees that discount has been applied

	# apply shipping estimate
	Scenario: Apply shipping estimate
		Given Nikon D300 in shopping cart
		When the user fills <reqiredData> in estimate shopping
		And the user applies shipping with first offered flat shipping rate
		Then the user sees that the shipping estimate has been applied
		|requiredData  |
		|Country       |
		|Region/State  |
		|PostCode      |

########### end of tests ################
