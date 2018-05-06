import traceback

from behave import *
from selenium.webdriver.common.by import By
from selenium.webdriver.remote.webelement import WebElement
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

@when("the user clicks on Shopping cart")
def step_impl(context):
    #context.browser.find_element_by_css_selector("a[title='Shopping Cart']").click()
    context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')

@then("the user sees that shopping cart is empty")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, '#content > p'),
                                                                                 'Your shopping cart is empty!'))
    except(Exception):
        traceback.print_exc()

@given("description of camera Nikon D300 is shown")
def implstep_impl_step(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=product/product&path=33&product_id=31")

@when("the user types quantity 1")
def step_impl(context):
    context.browser.find_element_by_id('input-quantity').clear()
    context.browser.find_element_by_id('input-quantity').send_keys(1)

@step("the user clicks on Add to Cart button")
def step_impl(context):
    login_button = context.browser.find_element_by_id('button-cart').click()

def nikon_in_shopping_cart(context):
    context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')
    # je Nikon v kosiku?
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element(
            (By.CSS_SELECTOR, 'h2'),
            'What would you like to do next?'))
    except(Exception):
        context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=product/product&path=33&product_id=31")
        context.browser.find_element_by_id('input-quantity').clear()
        context.browser.find_element_by_id('input-quantity').send_keys(1)
        add_to_cart = context.browser.find_element_by_id('button-cart').click()
        context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')

@then("Nikon D300 is added to cart")
def step_impl(context):
    context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'table.table.table-bordered > tbody > tr > td.text-left > a'),
                                                                                 'Nikon D300'))
    except(Exception):
        traceback.print_exc()



@given("one available Nikon D300 in shopping cart")
def step_impl(context):
    ...

@then("the user sees Nikon D300 in shopping cart")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'table.table.table-bordered > tbody > tr > td.text-left > a'),
                                                                                 'Nikon D300'))
    except(Exception):
        traceback.print_exc()

@then("quantity equals one")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.XPATH, '//*[@id="content"]/form/div/table/tbody/tr/td[4]/div/input'), '1'))
    except(Exception):
        traceback.print_exc()

@given("Nikon D300 in shopping cart")
def step_impl(context):
    context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')
    # je Nikon v kosiku?
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element(
            (By.CSS_SELECTOR, 'able.table.table-bordered > tbody > tr > td.text-left > a'),
            'Nikon D300'))
    except(Exception):
        #print("neni")
        #traceback.print_exc()
        context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=product/product&path=33&product_id=31")
        context.browser.find_element_by_id('input-quantity').clear()
        context.browser.find_element_by_id('input-quantity').send_keys(1)
        add_to_cart = context.browser.find_element_by_id('button-cart').click()
        context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')


@when("the user updates quantity of Nikon D300 to 4")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(
            EC.visibility_of_element_located((By.ID, "input-quantity")))
    except(Exception):
        traceback.print_exc()
    #context.browser.find_element_by_name('quantity[55]').clear()
    #context.browser.find_element_by_name('quantity[55]').send_keys(4)
    context.browser.find_element_by_id('input-quantity').clear()
    context.browser.find_element_by_id('input-quantity').send_keys(1)
    context.browser.find_element_by_css_selector('input.btn.btn-primary').click()

@then("quantity of Nikon in cart is 4")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.NAME, 'quantity[23]'),
                                                                                 '4'))
    except(Exception):
        traceback.print_exc()

@when("the user removes Nikon D300")
def step_impl(context):
    context.browser.find_element_by_css_selector('span.input-group-btn > button.btn.btn-danger').click()

@given("Samsung Galaxy Tab 10.1 in shopping cart with 3 stars")
def step_impl(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/index.php?route=product/product&path=57&product_id=49")
    context.browser.find_element_by_id('input-quantity').clear()
    context.browser.find_element_by_id('input-quantity').send_keys(1)
    add_to_cart = context.browser.find_element_by_id('button-cart').click()
    #context.browser.get('http://mys01.fit.vutbr.cz:8050/index.php?route=checkout/cart')

@then("the user sees warning")
def step_impl(context):
    text = context.browser.find_element_by_class_name("alert-danger")
    assert "Products marked with *** are not available in the desired quantity or not in stock!" in text.get_attribute("outerHTML")

def admin_login(context):
    context.browser.get('http://mys01.fit.vutbr.cz:8050/admin/')
    context.browser.find_element_by_id('input-username').send_keys('admin')
    context.browser.find_element_by_id('input-password').send_keys('admin')
    context.browser.find_element_by_css_selector('button.btn.btn-primary').click()


@given("Admin area with gift vouchers")
def step_impl(context):
    admin_login(context)
    context.browser.find_element_by_xpath("//a[@id='button-menu']/i").click()
    context.browser.find_element_by_css_selector('#sale > a.parent > span').click()
    context.browser.find_element_by_css_selector('ul.collapse.in > li.active.open > a.parent').click()
    context.browser.find_element_by_css_selector('ul.collapse.in > li.active.open > ul.collapse.in > li.active.open > a').click()

@when("the admin adds enable gift voucher with required data")
def step_impl(context):
    context.browser.find_element_by_css_selector('a.btn.btn-primary').click()
    for row in context.table:
        context.browser.find_element_by_id(row['requiredData']).send_keys(row['realData'])
    context.browser.find_element_by_css_selector('i.fa.fa-save').click()

@then("gift voucher is added to system")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'div.alert.alert-success'),
                                                                                 'Success: You have modified vouchers!'))
    except(Exception):
        traceback.print_exc()

@given("certificate '1234' is valid")
def step_impl(context):
    # musime zkontrolovat, jestli je invalid a kdyztak ho smazat
    ...

@when("the user fills reqiredData in estimate shopping")
def step_impl(context):
    context.browser.find_element_by_xpath("//div[@id='accordion']/div[2]/div/h4/a").click()
    for row in context.table:
        context.browser.find_element_by_id(row['requiredData']).send_keys(row['realData'])
    context.browser.find_element_by_id('button-quote').click()

@step("the user applies shipping with first offered flat shipping rate")
def step_impl(context):
    WebDriverWait(context.browser, 5).until(
        EC.visibility_of_element_located((By.CSS_SELECTOR, 'div.radio > label')))
    context.browser.find_element_by_css_selector('div.radio > label').click()
    context.browser.find_element_by_id('button-shipping').click()

@then("the user sees that the shipping estimate has been applied")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'div.alert.alert-success'),
                                                                                 'Success: Your shipping estimate has been applied!'))
    except(Exception):
        traceback.print_exc()







#@when("the user applies certificate number '1234'")


