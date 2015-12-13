cimport float_cmpl
from pprz_algebra_float_c cimport FloatVect3, FloatRates, FloatEulers, float_eulers_of_quat
import numpy as np

cdef class AhrsFloatCmpl:
    cdef float cur_time
    cdef float last_accel_update

    def __cinit__(self):
        float_cmpl.ahrs_fc_init()
        self.cur_time = 0.0
        self.last_accel_update = 0.0

    cpdef propagate(self, gyro, dt):
        cdef FloatRates r
        (r.p, r.q, r.r) = np.array(gyro)
        float_cmpl.ahrs_fc_propagate(&r, dt)
        self.cur_time += dt

    cpdef update_accel(self, accel):
        cdef FloatVect3 a
        (a.x, a.y, a.z) = np.array(accel)
        dt = self.cur_time - self.last_accel_update
        float_cmpl.ahrs_fc_update_accel(&a, dt)
        self.last_accel_update = self.cur_time

    cpdef update_mag(self, mag):
        cdef FloatVect3 m
        (m.x, m.y, m.z) = np.array(mag)
        dt = 1.0 / 512
        float_cmpl.ahrs_fc_update_mag(&m, dt)

    property quat:
        def __get__(self):
            q = float_cmpl.ahrs_fc.ltp_to_imu_quat
            return np.array([q.qi, q.qx, q.qy, q.qz], dtype='d')

    property euler:
        def __get__(self):
            cdef FloatEulers e
            float_eulers_of_quat(&e, &float_cmpl.ahrs_fc.ltp_to_imu_quat)
            return np.array([e.phi, e.theta, e.psi], dtype='d')
