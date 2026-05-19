import hashlib


def calculate_sha512(text):
    """
    Вычисляет SHA-512 хэш для введённого текста.
    """
    return hashlib.sha512(text.encode("utf-8")).hexdigest()


if __name__ == "__main__":
    user_text = input("Введите текст: ")
    print("SHA-512:", calculate_sha512(user_text))
