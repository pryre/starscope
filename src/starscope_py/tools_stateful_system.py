class StatefulSystem():
    def init(self):
        raise RuntimeError("Parent class does not implement an init method")

    def deinit(self):
        raise RuntimeError("Parent class does not implement an deinit method")
