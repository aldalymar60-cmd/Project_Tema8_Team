# Class Diagram

Диаграмма классов показывает основные классы проекта и связи между ними.

```mermaid
classDiagram
    class Client {
        +send_request(operation, data)
    }

    class Server {
        +home()
        +process()
    }

    class Database {
        +create_table()
        +save_operation(operation, input_data, result)
        +get_operations()
    }

    class ServerConfig {
        +host
        +port
        +debug
    }

    class RSA {
        +generate_keys()
        +encrypt(text, public_key)
        +decrypt(encrypted_text, private_key)
    }

    class SHA512 {
        +calculate_sha512(text)
    }

    class Bisection {
        +function(x)
        +bisection_method(a, b, epsilon)
    }

    class ShortestPath {
        +shortest_path(graph, start, end)
    }

    Client --> Server
    Server --> Database
    Server --> RSA
    Server --> SHA512
    Server --> Bisection
    Server --> ShortestPath
    Server --> ServerConfig
```

## Описание классов

- `Client` отправляет запросы на сервер.
- `Server` принимает запросы и возвращает результат.
- `Database` сохраняет историю операций.
- `ServerConfig` реализует шаблон Singleton.
- `RSA` выполняет шифрование и расшифрование.
- `SHA512` вычисляет хэш.
- `Bisection` находит корень уравнения методом деления пополам.
- `ShortestPath` ищет кратчайшее расстояние между вершинами графа.
