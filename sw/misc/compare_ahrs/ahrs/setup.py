#!/usr/bin/env python

"""
setup.py file for pprz AHRS wrappers
"""

from distutils.core import setup, Extension

from os import path, getenv

# if PAPARAZZI_SRC not set, then assume the tree containing this
# file is a reasonable substitute
pprz_src = getenv("PAPARAZZI_SRC", path.normpath(path.join(path.dirname(path.abspath(__file__)), '../../../../')))
pprz_airborne = path.join(pprz_src, "sw/airborne")

common_inc_dirs = ['./', path.join(pprz_src, "sw/include"), pprz_airborne, path.join(pprz_airborne, 'arch/sim')]

float_cmpl_module = Extension('_float_cmpl',
                              sources=['float_cmpl_wrap.c',
                                       path.join(pprz_airborne, 'subsystems/ahrs/ahrs_float_cmpl.c'),
                                       path.join(pprz_airborne, 'math/pprz_algebra_int.c'),
                                       path.join(pprz_airborne, 'math/pprz_algebra_double.c'),
                                       path.join(pprz_airborne, 'math/pprz_algebra_float.c'),
                                       path.join(pprz_airborne, 'math/pprz_orientation_conversion.c'),
                                       path.join(pprz_airborne, 'math/pprz_trig_int.c'),
                                      ],
                              include_dirs=common_inc_dirs,
                              extra_compile_args=["-std=c99", "-DAHRS_PROPAGATE_QUAT"])

setup(name='ahrs',
      version='0.1',
      author="Felix Ruess",
      description="""Pprz AHRS wrappers""",
      ext_modules=[float_cmpl_module],
      py_modules=["float_cmpl"],
      )
