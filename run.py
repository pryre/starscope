from src.starscope.starscope import Starscope

def debug():
    starscope = Starscope()

    try:
        print('Starting system...')
        starscope.init()
        print('System ready!')

        # Run background loop
        starscope.run()
    except KeyboardInterrupt:
        pass
    except Exception as e:
        raise e
    finally:
        starscope.deinit()
        print('System shutdown!')

if __name__ == '__main__':
    debug()
