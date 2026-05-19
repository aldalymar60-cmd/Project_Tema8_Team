import requests


SERVER_URL = "http://127.0.0.1:5000/process"


def send_request(operation, data):
    response = requests.post(SERVER_URL, json={
        "operation": operation,
        "data": data
    })
    return response.json()


if __name__ == "__main__":
    print("Client for Project Tema 8")
    print("1. RSA")
    print("2. SHA-512")
    print("3. Bisection method")
    print("4. Shortest path")

    choice = input("Choose operation: ")
    text = input("Enter data: ")

    result = send_request(choice, text)
    print("Result:", result)
