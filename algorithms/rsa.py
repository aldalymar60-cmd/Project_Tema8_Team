def gcd(a, b):
    """
    Находит наибольший общий делитель.
    """
    while b != 0:
        a, b = b, a % b
    return a


def mod_inverse(e, phi):
    """
    Находит обратное число для e по модулю phi.
    """
    for d in range(2, phi):
        if (d * e) % phi == 1:
            return d
    return None


def generate_keys():
    """
    Генерирует простую пару ключей RSA.
    Для учебного проекта используются маленькие простые числа.
    """
    p = 61
    q = 53

    n = p * q
    phi = (p - 1) * (q - 1)

    e = 17
    d = mod_inverse(e, phi)

    public_key = (e, n)
    private_key = (d, n)

    return public_key, private_key


def encrypt(text, public_key):
    """
    Шифрует текст с помощью открытого ключа RSA.
    """
    e, n = public_key
    encrypted = [pow(ord(char), e, n) for char in text]
    return encrypted


def decrypt(encrypted_text, private_key):
    """
    Расшифровывает текст с помощью закрытого ключа RSA.
    """
    d, n = private_key
    decrypted = ''.join(chr(pow(char, d, n)) for char in encrypted_text)
    return decrypted


if __name__ == "__main__":
    public_key, private_key = generate_keys()

    text = input("Введите текст: ")

    encrypted_text = encrypt(text, public_key)
    decrypted_text = decrypt(encrypted_text, private_key)

    print("Открытый ключ:", public_key)
    print("Закрытый ключ:", private_key)
    print("Зашифрованный текст:", encrypted_text)
    print("Расшифрованный текст:", decrypted_text)
