import heapq


def shortest_path(graph, start, end):
    """
    Поиск кратчайшего расстояния между вершинами графа.
    Используется алгоритм Дейкстры.
    graph = {
        "A": {"B": 4, "C": 2},
        "B": {"D": 5},
        "C": {"B": 1, "D": 8},
        "D": {}
    }
    """

    distances = {vertex: float("inf") for vertex in graph}
    distances[start] = 0

    previous = {vertex: None for vertex in graph}
    queue = [(0, start)]

    while queue:
        current_distance, current_vertex = heapq.heappop(queue)

        if current_vertex == end:
            break

        if current_distance > distances[current_vertex]:
            continue

        for neighbor, weight in graph[current_vertex].items():
            distance = current_distance + weight

            if distance < distances[neighbor]:
                distances[neighbor] = distance
                previous[neighbor] = current_vertex
                heapq.heappush(queue, (distance, neighbor))

    path = []
    current = end

    while current is not None:
        path.insert(0, current)
        current = previous[current]

    return {
        "distance": distances[end],
        "path": path
    }


if __name__ == "__main__":
    graph = {
        "A": {"B": 4, "C": 2},
        "B": {"D": 5},
        "C": {"B": 1, "D": 8},
        "D": {}
    }

    result = shortest_path(graph, "A", "D")

    print("Кратчайшее расстояние:", result["distance"])
    print("Путь:", " -> ".join(result["path"]))
