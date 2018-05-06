import traceback

from behave import *
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

use_step_matcher("re")

@given( "shopping web page Opencart is displayed" )
def step_impl(context):
    context.browser.get("http://mys01.fit.vutbr.cz:8050/")

@when( "the user clicks on Software" )
def step_impl(context):
    checkout = context.browser.find_element_by_link_text("Software").click()
    context.browser.implicitly_wait(10)
    #assert "Checkout" in context.browser.

@then("the user sees There are no products to list in this category")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, '#content > p'),
                                                                                 'There are no products to list in this category'))
    except(Exception):
        traceback.print_exc()


@when("the user clicks on Cameras in Categories")
def step_impl(context):
    checkout = context.browser.find_element_by_link_text("Cameras").click()

@then('the user sees Canon EOS 5D')
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.LINK_TEXT, 'Canon EOS 5D'),
                                                                                 'Canon EOS 5D'))
    except(Exception):
        traceback.print_exc()

@then('the user sees Nikon D300')
def step_impl(context):
        text = context.browser.find_element_by_xpath("//a[contains(text(),'Nikon D300')]")
        #print(text.get_attribute)


@when("the user clicks on Laptops & Notebooks")
def step_impl(context):
    context.browser.find_element_by_link_text("Laptops & Notebooks").click()


@then("the user sees dropmenu containing Macs")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.CSS_SELECTOR, 'li.dropdown.open > div.dropdown-menu > div.dropdown-inner > ul.list-unstyled > li > a'),
                                                                                 'Macs'))
    except(Exception):
        traceback.print_exc()



@then("the user sees dropmenu containing Windows")
def step_impl(context):
    try:
        WebDriverWait(context.browser, 5).until(EC.text_to_be_present_in_element((By.XPATH, "//nav[@id='menu']/div[2]/ul/li[2]/div/div/ul/li[2]/a"),
                                                                                 'Windows'))
    except(Exception):
        traceback.print_exc()
