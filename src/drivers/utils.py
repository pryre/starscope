import time
#from types import FunctionType
import time

class benchmark(object):
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
