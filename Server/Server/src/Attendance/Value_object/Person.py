
class Person:
    def __init__(self, _id: int = None, name: str = None):
        self._id: int = _id
        self.name: str = name

    def __str__(self):
        return f"(_id:{self._id}, name:{self.name})"
