from flask import Flask, request, jsonify
from database import Database

import sys
import os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from algorithms.sha512 import calculate_sha512
from algorithms.rsa import generate_keys, encrypt, decrypt
from algorithms.bisection import bisection_method
from algorithms.shortest_path import shortest_path


app = Flask(__name__)
db = Database()


@app.route("/")
def home():
    return "Server is running"


@app.route("/process", methods=["POST"])
def process():
    data = request.get_json()

    operation = data.get("operation")
    input_data = data.get("data")

    result = None

    try:
        if operation == "sha512":
            result = calculate_sha512(input_data)

        elif operation == "rsa_encrypt":
            public_key, private_key = generate_keys()
            encrypted = encrypt(input_data, public_key)
            result = {
                "public_key": public_key,
                "private_key": private_key,
                "encrypted_text": encrypted
            }

        elif operation == "rsa_decrypt":
            result = "RSA decrypt requires encrypted data and private key"

        elif operation == "bisection":
            a = float(input_data.get("a"))
            b = float(input_data.get("b"))
            epsilon = float(input_data.get("epsilon"))
            result = bisection_method(a, b, epsilon)

        elif operation == "shortest_path":
            graph = input_data.get("graph")
            start = input_data.get("start")
            end = input_data.get("end")
            result = shortest_path(graph, start, end)

        else:
            result = "Unknown operation"

        db.save_operation(operation, str(input_data), str(result))

        return jsonify({
            "operation": operation,
            "result": result
        })

    except Exception as error:
        return jsonify({
            "operation": operation,
            "error": str(error)
        })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)


