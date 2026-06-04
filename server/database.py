import sqlite3
from datetime import datetime


class Database:
    """
    Класс для работы с базой данных SQLite.
    Сохраняет историю выполненных операций.
    """

    def __init__(self, db_name="operations.db"):
        self.connection = sqlite3.connect(db_name, check_same_thread=False)
        self.create_table()

    def create_table(self):
        cursor = self.connection.cursor()
        cursor.execute("""
            CREATE TABLE IF NOT EXISTS operations (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                operation TEXT NOT NULL,
                input_data TEXT NOT NULL,
                result TEXT NOT NULL,
                created_at TEXT NOT NULL
            )
        """)
        self.connection.commit()

    def save_operation(self, operation, input_data, result):
        cursor = self.connection.cursor()
        cursor.execute("""
            INSERT INTO operations (operation, input_data, result, created_at)
            VALUES (?, ?, ?, ?)
        """, (operation, input_data, result, datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
        self.connection.commit()

    def get_operations(self):
        cursor = self.connection.cursor()
        cursor.execute("SELECT * FROM operations")
        return cursor.fetchall()


