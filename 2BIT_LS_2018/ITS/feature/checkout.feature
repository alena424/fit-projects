# ITS
# 1. project based on BDD testing
# @date		28.03.2018
# @author	Alena Tesarova, xtesar36 

# Shopping process 3/3

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
		
		Examples:
		|requiredData     |realData       |
		|FirstName        |Alena          |
		|LastName         |Tesarova       |
		|Email            |Alisa@seznam.cz|
		|Telephone        |605405609      |
		|Address1         |Obla	34        |
		|City             |Brno           |
		|PostCode         |64000          |
		|Country          |United Kingdom |
		|Region           |Bristol        |
		|Password         |ae34lt6        |
		|Password Confirm |ae34lt6        |

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
			|requireItems |
			|FirstName    |
			|LastName     |
			|Address1     |
			|City         |
			|PostCode     |
			|Country      |
			|Region       |

	# trying to fill too long name
	Scenario: Try to fill long name
		Given step 2 Billing Details is expanded
		When the user fills First name of 129 caracters
		Then the user gets warning
	
	Scenario: Continue to step 3
		Given step 2 Billing Deatils is expanded
		When the user checks "I want to use an existing address"
		And the user clicks "Continue"
		Then step 3 expands with Delivery Details
		
	Scenario: Get from step 3 to step 4
		Given step 3 Deliver Details is expanded
		And the user checks "I want to use an existing address"
		When the user clicks on "Continue"
		Then step 4 expands with Delivery Method

	Scenario: get from step 4 to step 5
		Given step 4 Delivery method is expanded
		And the user checks first option Flat shipping Rate
		And the user clicks on "Continue"
		Then step 5 expands with Payment Method

	# try to continue without reading terms and condition
	Scenario: Try to continue without checking reading terms and condition
		Given Step 5 Payment method is expanded
		And reading terms and condition is not checked
		When the user clicks on Continue
		Then the user gets warning

	Scenario: get from step 5 to step 6
		Given step 5 Payment method is expanded
		When the users checks reading terms and condition
		Then step 6 expands with Confirm order

	# confirm
	Scenario: Confirm order
		Given the user is in Step 6
		When the user clicks on "Continue"
		Then order is done


########### end of tests ################
