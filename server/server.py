from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route("/")
def home():
    return "Server is running"

@app.route("/process", methods=["POST"])
def process():
    data = request.get_json()
    operation = data.get("operation")

    return jsonify({
        "operation": operation,
        "result": "Function will be implemented later"
    })

if __name__ == "__main__":
    app.run(debug=True)
