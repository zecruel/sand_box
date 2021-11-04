import time

from selenium import webdriver
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait


driver = webdriver.Firefox(executable_path='/home/ezequiel/webdriver/geckodriver')  # Optional argument, if not specified will search path.

driver.get('http://www.google.com/');

time.sleep(5) # Let the user actually see something!

search_box = driver.find_element(By.NAME,'q')

#aguarda até o elemento com certo ID ser clicável
#wait.until(EC.element_to_be_clickable((By.ID, 'id_do_elemento')))


#title_is
#title_contains
#presence_of_element_located
#visibility_of_element_located
#visibility_of
#presence_of_all_elements_located
#text_to_be_present_in_element
#text_to_be_present_in_element_value
#frame_to_be_available_and_switch_to_it
#invisibility_of_element_located
#element_to_be_clickable
#staleness_of
#element_to_be_selected
#element_located_to_be_selected
#element_selection_state_to_be
#element_located_selection_state_to_be
#alert_is_present


search_box.send_keys('ChromeDriver')

search_box.submit()

time.sleep(5) # Let the user actually see something!

driver.quit()