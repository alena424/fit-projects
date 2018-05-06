# ITS
# 1. project based on BDD testing
# @date		28.03.2018
# @author	Alena Tesarova, xtesar36

# Shopping process 3/3

Feature: Check out

	# get to checkout page
	Scenario: Get to checkout page
		Given shopping cart is shown
		And something in cart
		When the user clicks on Checkout
		Then the user gets to page with title Checkout

	# register
	Scenario: To register
	Given Register account page
		When the user fills requiredData with realData
        |requiredData           |realData       |
		|input-firstname        |Alena          |
		|input-lastname         |Tesarova       |
		|input-email            |Alisa@seznam.cz|
		|input-telephone        |605405609      |
		|input-address-1        |Obla	34      |
		|input-city             |Brno           |
		|input-postcode         |64000          |
		|input-country          |United Kingdom |
		|input-zone             |Bristol        |
		|input-password         |ae34lt6        |
		|input-confirm          |ae34lt6        |

		And the user agrees to Privacy Policy
		And the user clicks on Continue
		Then an account is created


	# returning customer logs in
	Scenario: Login returning customer
		Given Login page
		When the user logs in as a returning customer
        |requiredData           |realData       |
        |input-email            |Alisa@seznam.cz|
        |input-password         |ae34lt6        |
		And clicks on Login
		Then the user loggs in

		# trying to skip required data
		Scenario: Try to skip required data
			Given the registered user is in Step 2 Billing Details
			And an option 'I want to use a new address' is selected
			When the user continues without filling "input-payment-firstname"
			|requiredData           		|realData       |
			|input-payment-firstname        |Alena          |
			|input-payment-lastname         |Tesarova       |
			|input-payment-address-1        |Obla	34      |
			|input-payment-city             |Brno           |
			|input-payment-postcode         |64000          |
			|input-payment-country          |United Kingdom |
			|input-payment-zone             |Bristol        |
			Then the user gets warning by name

	# trying to fill too long name
	#Scenario: Try to fill long name
	#	Given step 2 Billing Details is expanded
	#	When the user fills First too long name
	#	|requiredData           |realData       |
	#	|input-firstname        |Alenaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa|
	#	Then the user gets warning by name

	Scenario: Continue to step 3
		Given the registered user is in Step 2 Billing Details
		When the user checks 'I want to use an existing address'
		And the user clicks 'Continue'
		Then step 3 expands with Delivery Details

	Scenario: Get from step 3 to step 4
		Given step 3 Deliver Details is expanded
		And the user checks 'I want to use an existing address' in step3
		When the user clicks on 'Continue' in step3
		Then step 4 expands with Delivery Method

	Scenario: get from step 4 to step 5
		Given step 4 Delivery method is expanded
		And the user checks first option Flat shipping Rate
		And the user clicks on 'Continue' in step 4
		Then step 5 expands with Payment Method

	# try to continue without reading terms and condition
	#Scenario: Try to continue without checking reading terms and condition
	#	Given Step 5 Payment method is expanded
	#	And reading terms and condition is not checked
	#	When the user clicks on Continue
	#	Then the user gets warning

	Scenario: get from step 5 to step 6
		Given step 5 Payment method is expanded
		When the user agrees to Privacy Policy
		And the user clicks on 'Continue' in step 5
		Then step 6 expands with Confirm order

	# confirm
	Scenario: Confirm order
		Given the user is in Step 6
		When the user clicks on 'Continue' in step 6
		Then order is done


########### end of tests ################
