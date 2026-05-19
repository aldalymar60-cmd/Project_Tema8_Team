from server.singleton import ServerConfig


def test_singleton_instance():
    """
    Проверка, что ServerConfig создаёт только один экземпляр.
    """

    config1 = ServerConfig()
    config2 = ServerConfig()

    assert config1 is config2


if __name__ == "__main__":
    test_singleton_instance()
    print("Singleton test passed")
