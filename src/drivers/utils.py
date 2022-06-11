import time
#from types import FunctionType
import time

class Size:
    def __init__(self, x:int=0, y:int=0) -> None:
        self.x = x
        self.y = y

    def __repr__(self):
        return "Size: [{},{}]".format(self.x, self.y)

    def __str__(self):
        return "Size: [{},{}]".format(self.x, self.y)

class Vector3():
    def __init__(self, x:float = 0, y:float = 0, z:float = 0) -> None:
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self) -> str:
        return "Vector3()"

    def __str__(self) -> str:
        return f'[{self.x:0.4f}, {self.y:0.4f}, {self.z:0.4f}]'

class Benchmark(object):
    def __init__(self, msg:str):
        # self.fmt = msg + ": {}us"
        self.msg = msg

    def __enter__(self):
        self.start = time.ticks_cpu()
        return self

    def __exit__(self, *args):
        t = time.ticks_cpu() - self.start
        print(f'{self.msg}: {t}us')
        self.time = t
