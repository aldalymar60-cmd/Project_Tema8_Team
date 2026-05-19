def function(x):
    """
    Функция для поиска корня.
    Пример: f(x) = x^2 - 4
    Корень уравнения равен x = 2 или x = -2.
    """
    return x ** 2 - 4


def bisection_method(a, b, epsilon=0.0001):
    """
    Метод деления пополам для нахождения корня уравнения f(x) = 0.
    """

    if function(a) * function(b) >= 0:
        return "Ошибка: на данном отрезке функция не меняет знак."

    while abs(b - a) > epsilon:
        middle = (a + b) / 2

        if function(middle) == 0:
            return middle

        if function(a) * function(middle) < 0:
            b = middle
        else:
            a = middle

    return (a + b) / 2


if __name__ == "__main__":
    a = float(input("Введите a: "))
    b = float(input("Введите b: "))
    epsilon = float(input("Введите точность epsilon: "))

    result = bisection_method(a, b, epsilon)

    print("Результат:", result)
