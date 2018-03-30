# ITS
# 1. project based on BDD testing
# @date		28.03.2018
# @author	Alena Tesarova, xtesar36 

# Shopping process

Feature: Choosing item

	# The user wants to buy something
	Scenario: Display a shopping web page
		Given wiki ITS page with Opencart URL  
		When the user clicks on "Opencart" online shop
		Then the user sees "Opencart" title

	# user wants to find what he wants to buy
 	Scenario: Find cameras
		Given shopping web Opencart is shown
		When the user clicks on Cameras in Categories
		Then the user sees cameras "Canon EOS 5D" and "Nikon D300"

	# user wants to add item to cart
	Scenario: Adding Nikon to cart
		Given description of camera "Nikon D300" is shown
		When the user types quantity 1
		And the user clicks on "Add to Cart" button
		Then Nikon D300 is added to cart

Feature: Shopping cart

	# shopping cart empty
	Scenario: Display shopping cart
		Given no product in shopping cart
		When the user clicks on "Shopping cart"
		Then the user sees that shopping cart is empty
	
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
		Given Samsung Galaxy Tab 10.1 in shopping cart
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
		And the user applies shipping
		Then the user sees that the shipping estimate has been applied

Feature: Check out

	# get to checkout page
	Scenario: Get to checkout page
		Given shopping cart is shown
		When the user clicks on "Checkout"
		Then the user gets to page with title "Checkout"
	
	#	register
	Scenario: To register
		Given Register account page
		When the user fills <requiredData> with <realData>
		And the user agreeds to Privacy Policy
		And the user clicks on Continue
		Then an account is created
		|requiredData			|realData				|
		|FirstName				|Alena					|
		|LastName					|Tesarova				|
		|Email						|Alisa@seznam.cz|
		|Telephone				|605405606			|
		|Address1					|Obla	34				|
		|City							|Brno						|
		|PostCode					|64000					|
		|Country					|United Kingdom	|
		|Region						|Bristol				|
		|Password					|ae34lt6				|
		|Password Confirm |ae34lt6				|

	# returning customer logs in
	Scenario: Login returning customer
		Given the user is a registered customer
		When the user logins as a returning customer
		And clicks on "Login"
		Then the user loggs in

		# trying to skip required data
		Scenario: Try to skip required data
			Given the registered user is in Step 2 Billing Details
			And an option "I want to use a new address" is selected
			When the user continues without filling <requireItems>
			Then the user sees warning
			|requireItems	|
			|FirstName		|
			|LastName			|
			|Address1			|
			|City					|
			|PostCode			|
			|Country			|
			|Region				|

	# trying to fill too long name
	Scenario: Try to fill long name
		Given Step 2 Billing Details is expanded
		When the user fills First name of 129 caracters
		Then the user gets warning

	Scenario: Get from step 3 to step 4
		Given Step 3 Deliver Details is expanded
		When the user clicks on "Continue"
		Then step 4 expands with Delivery Method

	# try to continue without reading terms and condition
	Scenario: Try to continue without checking reading terms and condition
		Given the user is in Step 5
		And reding terms and condition is not checked
		When the user clicks on Continue
		Then the user gets warning


	# confirm
	Scenario: Confirm order
		Given the user is in Step 6
		When the user clicks on "Continue"
		Then order is done


########### end of tests ################
