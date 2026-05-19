import requests


SERVER_URL = "http://127.0.0.1:5000/process"


def send_request(operation, data):
    response = requests.post(SERVER_URL, json={
        "operation": operation,
        "data": data
    })
    return response.json()


def main():
    print("Client for Project Tema 8")
    print("1. RSA Encrypt")
    print("2. SHA-512")
    print("3. Bisection method")
    print("4. Shortest path")

    choice = input("Choose operation: ")

    if choice == "1":
        text = input("Enter text for RSA encryption: ")
        result = send_request("rsa_encrypt", text)

    elif choice == "2":
        text = input("Enter text for SHA-512: ")
        result = send_request("sha512", text)

    elif choice == "3":
        a = input("Enter a: ")
        b = input("Enter b: ")
        epsilon = input("Enter epsilon: ")

        result = send_request("bisection", {
            "a": a,
            "b": b,
            "epsilon": epsilon
        })

    elif choice == "4":
        graph = {
            "A": {"B": 4, "C": 2},
            "B": {"D": 5},
            "C": {"B": 1, "D": 8},
            "D": {}
        }

        start = input("Enter start vertex, for example A: ")
        end = input("Enter end vertex, for example D: ")

        result = send_request("shortest_path", {
            "graph": graph,
            "start": start,
            "end": end
        })

    else:
        result = {"error": "Unknown choice"}

    print("Result:")
    print(result)


if __name__ == "__main__":
    main()
