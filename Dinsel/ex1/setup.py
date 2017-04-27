from distutils.core import setup
from Cython.Build import cythonize
import numpy
# import sys
#
# def goCompile(fname):
#     """Insert fname into setup."""
fname = 'cGetData.pyx'
# fname = '/OPTI/bzfdinse/trr154-dev/Z02/tools/convert_example/convertNet.pyx'
setup(
    name=fname.split('.')[0],
    ext_modules=cythonize(fname),
    )

# def main(argv):
#     """handle stuff."""
#     import argparse
#     parser = argparse.ArgumentParser(description="Try to compile python to cython with given .py")
#
#     parser.add_argument('file', help='file to cythonize', action='store')
#
#     goCompile(parser.parse_args())
#
#
# if __name__ == '__main__':
#     main(sys.argv[1:])
