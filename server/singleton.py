class SingletonMeta(type):
    """
    Метакласс Singleton.
    Гарантирует, что у класса будет только один экземпляр.
    """

    _instances = {}

    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(SingletonMeta, cls).__call__(*args, **kwargs)
        return cls._instances[cls]


class ServerConfig(metaclass=SingletonMeta):
    """
    Класс конфигурации сервера, реализованный по шаблону Singleton.
    """

    def __init__(self):
        self.host = "127.0.0.1"
        self.port = 5000
        self.debug = True
