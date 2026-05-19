import tkinter as tk
from tkinter import messagebox
import requests


SERVER_URL = "http://127.0.0.1:5000/process"


class ClientSingleton:
    """
    Singleton-класс клиента.
    Используется для отправки запросов на сервер.
    """

    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(ClientSingleton, cls).__new__(cls)
        return cls._instance

    def send_request(self, operation, data):
        response = requests.post(SERVER_URL, json={
            "operation": operation,
            "data": data
        })
        return response.json()


class ClientGUI:
    """
    Оконный интерфейс клиента.
    """

    def __init__(self, root):
        self.client = ClientSingleton()
        self.root = root
        self.root.title("Project Tema 8 Client")
        self.root.geometry("600x500")

        self.title_label = tk.Label(root, text="Project Tema 8", font=("Arial", 20, "bold"))
        self.title_label.pack(pady=10)

        self.input_label = tk.Label(root, text="Input data:")
        self.input_label.pack()

        self.input_text = tk.Text(root, height=5, width=60)
        self.input_text.pack(pady=5)

        self.result_label = tk.Label(root, text="Result:")
        self.result_label.pack()

        self.result_text = tk.Text(root, height=10, width=60)
        self.result_text.pack(pady=5)

        self.buttons_frame = tk.Frame(root)
        self.buttons_frame.pack(pady=10)

        tk.Button(self.buttons_frame, text="RSA Encrypt", width=20, command=self.rsa_encrypt).grid(row=0, column=0, padx=5, pady=5)
        tk.Button(self.buttons_frame, text="SHA-512", width=20, command=self.sha512).grid(row=0, column=1, padx=5, pady=5)
        tk.Button(self.buttons_frame, text="Bisection", width=20, command=self.bisection).grid(row=1, column=0, padx=5, pady=5)
        tk.Button(self.buttons_frame, text="Shortest Path", width=20, command=self.shortest_path).grid(row=1, column=1, padx=5, pady=5)

    def show_result(self, result):
        self.result_text.delete("1.0", tk.END)
        self.result_text.insert(tk.END, str(result))

    def get_input(self):
        return self.input_text.get("1.0", tk.END).strip()

    def rsa_encrypt(self):
        text = self.get_input()
        result = self.client.send_request("rsa_encrypt", text)
        self.show_result(result)

    def sha512(self):
        text = self.get_input()
        result = self.client.send_request("sha512", text)
        self.show_result(result)

    def bisection(self):
        data = {
            "a": 1,
            "b": 3,
            "epsilon": 0.0001
        }
        result = self.client.send_request("bisection", data)
        self.show_result(result)

    def shortest_path(self):
        graph = {
            "A": {"B": 4, "C": 2},
            "B": {"D": 5},
            "C": {"B": 1, "D": 8},
            "D": {}
        }

        data = {
            "graph": graph,
            "start": "A",
            "end": "D"
        }

        result = self.client.send_request("shortest_path", data)
        self.show_result(result)


if __name__ == "__main__":
    root = tk.Tk()
    app = ClientGUI(root)
    root.mainloop()
