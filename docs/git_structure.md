# Git Structure

Данный файл описывает структуру репозитория проекта.

```text
Project_Tema8_Team/
│
├── README.md
├── requirements.txt
├── Dockerfile
├── docker-compose.yml
│
├── server/
│   ├── server.py
│   ├── database.py
│   ├── singleton.py
│   └── services.py
│
├── client/
│   └── client.py
│
├── algorithms/
│   ├── rsa.py
│   ├── sha512.py
│   ├── bisection.py
│   └── shortest_path.py
│
├── docs/
│   ├── project_description.md
│   ├── test_plan.md
│   ├── use_case_diagram.md
│   ├── class_diagram.md
│   └── git_structure.md
│
└── tests/
    └── test_singleton.py
```

## Описание структуры

- `server/` — серверная часть приложения.
- `client/` — клиентская часть приложения.
- `algorithms/` — реализация алгоритмов темы №8.
- `docs/` — документация проекта, диаграммы и тест-план.
- `tests/` — тесты проекта.
- `README.md` — основное описание проекта.
- `Dockerfile` и `docker-compose.yml` — файлы для запуска проекта через Docker.
