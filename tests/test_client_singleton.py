import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from client.client_gui import ClientSingleton


def test_client_singleton_instance():
    """
    Проверка, что ClientSingleton создаёт только один экземпляр.
    """

    client1 = ClientSingleton()
    client2 = ClientSingleton()

    assert client1 is client2


if __name__ == "__main__":
    test_client_singleton_instance()
    print("Client Singleton test passed")
