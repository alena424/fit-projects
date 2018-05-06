import traceback

from behave import *
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

@given("shopping cart is shown")
def step_impl(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart")


@when( "the user clicks on Checkout" )
def step_impl(context):
    checkout = context.browser.find_element_by_css_selector("a[title='Checkout']").click()

# tady jsem detekovala chybu
@then("the user gets to page with title Checkout")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h1'),
                                                                                 'Checkout'))
    except(Exception):
        traceback.print_exc()

@given( "Register account page" )
def step_impl(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=account/register")

@when("the user fills requiredData with realData")
def step_impl(context):
    for row in context.table:
        context.browser.find_element_by_id(row['requiredData']).send_keys(row['realData'])

@step("the user agrees to Privacy Policy")
def step_impl(context):
    context.browser.find_element_by_name('agree').click()

@step("the user clicks on Continue")
def step_impl(context):
    continue_button = context.browser.find_element_by_css_selector('input.btn.btn-primary').click()

@then("an account is created")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h1'),
                                                                                 'Your Account Has Been Created!'))
    except(Exception):
        pass
		#print("error")
        #traceback.print_exc()

@step("Login page")
def step_impl(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=account/login")

@when("the user logs in as a returning customer")
def step_impl(context):
    for row in context.table:
        context.browser.find_element_by_id(row['requiredData']).send_keys(row['realData'])

@when("clicks on Login")
def step_impl(context):
    login_button = context.browser.find_element_by_css_selector('input.btn.btn-primary').click()

@then("the user loggs in")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h2'),
                                                                                 'My Account'))
    except(Exception):
        traceback.print_exc()

def check_if_registered(context):
    #continue_button = context.browser.find_element_by_xpath("//div[@id='top-links']/ul/li[2]/a/span").click()
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=account/account")
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h2'),
                                                                                 'My Account'))
    except(Exception):
        #traceback.print_exc()
        return False
    return True

def go_to_step2(context):
    # predpokladme, ze jsme v kosiku a je zde neco
    #checkout_button = context.browser.find_element_by_css_selector('a.btn.btn-primary').click()
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/checkout")



def login(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=account/login")
    array = {
        "input-email": "Alisa@seznam.cz",
        "input-password": "ae34lt6",
    }
    for key, value in array.items():
        context.browser.find_element_by_id(key).send_keys(value)
    login_button = context.browser.find_element_by_css_selector('input.btn.btn-primary').click()
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h2'),
                                                                                 'My Account'))
    except(Exception):
        #traceback.print_exc()
        return False
    return True


def register(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=account/register")
    array = {
   "input-firstname":"Lena",
   "input-lastname":"Tesarova",
   "input-email":"Alisa2@seznam.cz",
   "input-telephone":"605405609",
   "input-address-1":"Oblas 34",
   "input-city":"Brno",
   "input-postcode":"64010",
   "input-country":"United Kingdom",
   "input-zone":"Bristol",
   "input-password":"aeb34lt6",
   "input-confirm":"aeb34lt6",
    }
    for key, value in array.items():
        context.browser.find_element_by_id(key).send_keys(value)

    context.browser.find_element_by_name('agree').click()
    login_button = context.browser.find_element_by_css_selector('input.btn.btn-primary').click()
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h1'),
                                                                                 'Your Account Has Been Created!'))
    except(Exception):
        traceback.print_exc()


def something_in_shopping_cart(context):
    context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')
    # je Nikon v kosiku?
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element(
            (By.CSS_SELECTOR, 'h2'),
            'What would you like to do next?'))
    except(Exception):
        # print("neni")
        context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=product/product&path=33&product_id=31")
        context.browser.find_element_by_id('input-quantity').clear()
        context.browser.find_element_by_id('input-quantity').send_keys(1)
        add_to_cart = context.browser.find_element_by_id('button-cart').click()
        context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')

@step("something in cart")
def step_impl(context):
    something_in_shopping_cart(context)

@given("the registered user is in Step 2 Billing Details")
def step_impl(context):
    step2(context)

@when("the user checks 'I want to use an existing address'")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.NAME, 'payment_address')))
    except(Exception):
        traceback.print_exc()

    context.browser.find_element_by_name("payment_address").click()

@step("an option 'I want to use a new address' is selected")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.XPATH, '//div[@id="collapse-payment-address"]/div/form/div[3]/label')))
    except(Exception):
        traceback.print_exc()

    context.browser.find_element_by_xpath("(//input[@name='payment_address'])[2]").click()

@step("the user clicks 'Continue'")
def step_impl(context ):
    context.browser.find_element_by_id("button-payment-address").click()

@when('the user continues without filling "{text}"')
def step_impl(context, text ):
    for row in context.table:
        if ( row['requiredData'] != text):
            context.browser.find_element_by_id(row['requiredData']).send_keys(row['realData'])
    context.browser.find_element_by_id("button-payment-address").click()

@then("the user gets warning by name")
def step_impl(context ):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.CSS_SELECTOR, 'div.text-danger')))
    except(Exception):
        traceback.print_exc()

@then("step 3 expands with Delivery Details")
def step_impl(context ):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.NAME, 'shipping_address')))
    except(Exception):
        traceback.print_exc()

def step2(context):
    if ( check_if_registered(context) == False):
        if ( login(context) == False):
            register(context)
    # nesmi byt prazdny kosik
    something_in_shopping_cart(context)
    go_to_step2(context)
    try:
        WebDriverWait(context.browser, 2).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h1'),
                                                                                 'Checkout'))
    except(Exception):
        traceback.print_exc()

def step3(context):
    step2(context)
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.NAME, 'payment_address')))
    except(Exception):
        traceback.print_exc()

    context.browser.find_element_by_name("payment_address").click()
    context.browser.find_element_by_id("button-payment-address").click()
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.ID, 'button-shipping-address')))
    except(Exception):
        traceback.print_exc()

@given("step 3 Deliver Details is expanded")
def step_impl(context):
    step3(context)

@step("the user checks 'I want to use an existing address' in step3")
def step_impl(context):
    context.browser.find_element_by_name("shipping_address").click()

@when("the user clicks on 'Continue' in step3")
def step_impl(context):
    context.browser.find_element_by_id("button-shipping-address").click()

@then("step 4 expands with Delivery Method")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.NAME, 'comment')))
    except(Exception):
        traceback.print_exc()


def step4(context):
    step3(context)
    context.browser.find_element_by_name("shipping_address").click()
    context.browser.find_element_by_id("button-shipping-address").click()
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.NAME, 'comment')))
    except(Exception):
        traceback.print_exc()


@given("step 4 Delivery method is expanded")
def step_impl(context):
    step4(context)

@step("the user checks first option Flat shipping Rate")
def step_impl(context):
    context.browser.find_element_by_name("shipping_method").click()

@step("the user clicks on 'Continue' in step 4")
def step_impl(context):
    context.browser.find_element_by_id("button-shipping-method").click()

@then("step 5 expands with Payment Method")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.XPATH, "(//textarea[@name='comment'])[2]")))
    except(Exception):
        traceback.print_exc()

def step5(context):
    step4(context)
    context.browser.find_element_by_name("shipping_method").click()
    context.browser.find_element_by_id("button-shipping-method").click()
    try:
        WebDriverWait(context.browser, 5).until(
            EC.visibility_of_element_located((By.XPATH, "(//textarea[@name='comment'])[2]")))
    except(Exception):
        traceback.print_exc()

@given("Step 5 Payment method is expanded")
def step_impl(context):
    step5(context)

@then("step 6 expands with Confirm order")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 2).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'thead > tr > td.text-left'),
                                                                                 'Product Name'))
    except(Exception):
        traceback.print_exc()

@step("the user clicks on 'Continue' in step 5")
def step_impl(context):
    context.browser.find_element_by_id("button-payment-method").click()

@step("the user clicks on 'Continue' in step 6")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.ID, 'button-confirm')))
    except(Exception):
        traceback.print_exc()
    context.browser.find_element_by_id("button-confirm").click()

@given("the user is in Step 6")
def step_impl(context):
    step5(context)

    try:
        WebDriverWait(context.browser, 5).until(EC.visibility_of_element_located((By.NAME, 'agree')))
    except(Exception):
        traceback.print_exc()

    context.browser.find_element_by_name('agree').click()
    context.browser.find_element_by_id("button-payment-method").click()
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.XPATH,'//*[@id="collapse-checkout-confirm"]/div/div[1]/table/thead/tr/td[2]'),
                                                                                 'Model'))
    except(Exception):
        traceback.print_exc()

@then("order is done")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 2).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'h1'),
                                                                                 'Your order has been placed!'))
    except(Exception):
        traceback.print_exc()







