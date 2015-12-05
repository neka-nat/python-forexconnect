import os
import getpass
import base64
import pickle

CACHE_FILE_NAME = "/tmp/.logincache"

def get_login_params():
    if os.path.isfile(CACHE_FILE_NAME):
        return get_from_cache()
    else:
        return get_from_console()

def get_from_console():
    username = raw_input("username: ")
    password = getpass.getpass("password: ")
    connection = raw_input("connection: ")
    save_cache(username, password, connection)
    return username, password, connection

def get_from_cache():
    username, password, connection = pickle.load(open(CACHE_FILE_NAME))
    password = base64.b64decode(password)
    return username, password, connection

def save_cache(username, password, connection):
    pickle.dump((username, base64.b64encode(password), connection), open(CACHE_FILE_NAME, "w"))

def clear_cache():
    try:
        os.remove(CACHE_FILE_NAME)
    except:
        pass
