import sys
from os import path

if __name__ == '__main__':
    sys.path.append(path.join(path.dirname(__file__),
        '..', '..', 'src', 'python'))
    from das.gen_for_each.gen_for_each import GenForEach
    GenForEach(argv=sys.argv).run()
