
class Room:
    def __init__(self, _id: int = None, name: str = None, webcam_device_id: int = None):
        self._id: int = _id
        self.name: str = name
        self.webcam_device_id: int = webcam_device_id

    def __str__(self):
        return f"(_id:{self._id}, name:{self.name}, webcam_device_id:{self.webcam_device_id})"
