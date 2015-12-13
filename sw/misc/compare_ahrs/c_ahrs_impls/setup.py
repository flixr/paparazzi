from distutils.core import setup, Extension
from Cython.Build import cythonize
from distutils.extension import Extension
import numpy

from os import path, getenv

# if PAPARAZZI_SRC not set, then assume the tree containing this
# file is a reasonable substitute
pprz_src = getenv("PAPARAZZI_SRC", path.normpath(path.join(path.dirname(path.abspath(__file__)), '../../../../')))
pprz_airborne = path.join(pprz_src, "sw/airborne")

common_inc_dirs = ["./",
                   path.join(pprz_src, "sw/include"),
                   pprz_airborne,
                   path.join(pprz_airborne, "arch/sim"),
                   numpy.get_include()]

includedirs = common_inc_dirs
ext_ahrs_float_cmpl = Extension("ahrs_float_cmpl",
                                sources=['ahrs_float_cmpl.pyx',
                                         path.join(pprz_airborne, 'math/pprz_algebra_float.c'),
                                         path.join(pprz_airborne, 'math/pprz_algebra_int.c'),
                                         path.join(pprz_airborne, 'math/pprz_trig_int.c'),
                                         path.join(pprz_airborne, 'math/pprz_orientation_conversion.c'),
                                         path.join(pprz_airborne, "subsystems/ahrs/ahrs_float_cmpl.c"),],
                                include_dirs=includedirs,
                                extra_compile_args=["-std=c99", "-DAHRS_PROPAGATE_QUAT"])

extensions = [ext_ahrs_float_cmpl]

setup(
    ext_modules=cythonize(extensions)
)
