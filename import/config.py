import configparser

def load_config():
    config = configparser.ConfigParser()
    config.read('config.txt')
    return config

def get_city():
    return load_config()['Graph Settings']['city']