
all:
	swig2.0 -python -keyword -I../../../lib/python/pprz_math -I../../../airborne -I../../../airborne/arch/sim float_cmpl.i
	python setup.py build_ext --inplace

clean:
	rm -rf build *.so *.c *.pyc float_cmpl.py
