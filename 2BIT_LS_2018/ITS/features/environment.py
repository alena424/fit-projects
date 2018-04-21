from behave import fixture, use_fixture
from selenium.webdriver import Firefox

@fixture
def browser_firefox(context):
	# -- BEHAVE-FIXTURE: Similar to @contextlib.contextmanager
	context.browser = Firefox()
	yield context.browser
	# -- CLEANUP-FIXTURE PART:
	context.browser.quit()

def before_all(context):
		use_fixture(browser_firefox, context)
	# -- NOTE: CLEANUP-FIXTURE is called after after_all() hook.


