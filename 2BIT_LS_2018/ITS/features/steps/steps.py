# ITS
# 2. project based on BDD testing
# @author Alena Tesarova, xtesar36

# Shopping process 1/3
from behave import given, when, then

@given('shopping web page Opencart is displayed')
def step_impl(context):
	context.browser.get('http://mys01.fit.vutbr.cz:8050')

@when('the user clicks on "{Software}"')
def step_impl(context):
	pass

@then('the user sees "{The are no products to list in this category.}" ')
def step_impl(context):
	elements = find_elements( context.browser,id='content' )
	pass

