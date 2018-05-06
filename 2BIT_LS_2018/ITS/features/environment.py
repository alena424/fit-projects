from selenium import webdriver

#def before_all(context):
     #print("Executing before all")

#def before_feature(context, feature):
     #print("Before feature\n")

#Scenario level objects are popped off context when scenario exits
from selenium.webdriver import DesiredCapabilities


def before_scenario(context,scenario):
    #context.browser = webdriver.Chrome()
    context.browser = webdriver.Remote(
        command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',
        desired_capabilities=DesiredCapabilities.CHROME)
    context.browser.implicitly_wait(30)
    context.base_url = "http://www.fit.vutbr.cz/"
    context.verificationErrors = []
    context.accept_next_alert = True

    #print("Before scenario\n")

def after_scenario(context,scenario):
    context.browser.quit()
    #print("After scenario\n")

#def after_feature(context,feature):
     #print("\nAfter feature")

#def after_all(context):
     #print("Executing after all")

