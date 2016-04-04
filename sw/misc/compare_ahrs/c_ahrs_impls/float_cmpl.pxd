from pprz_algebra_float_c cimport FloatQuat, FloatEulers, FloatRates, FloatRMat, FloatVect3
from libc.stdint cimport uint8_t
# apparently that also covers C99 bool
from libcpp cimport bool

cdef extern from "subsystems/ahrs/ahrs_float_cmpl.h":
    struct AhrsFloatCmpl:
        FloatRates gyro_bias
        FloatRates rate_correction
        FloatRates imu_rate
        FloatQuat ltp_to_imu_quat
        FloatRMat ltp_to_imu_rmat

        bool correct_gravity
        float ltp_vel_norm
        bool ltp_vel_norm_valid

        float accel_omega
        float accel_zeta
        float mag_omega
        float mag_zeta

        uint8_t gravity_heuristic_factor
        float weight

        bool heading_aligned;
        FloatVect3 mag_h

        #uint16_t accel_cnt
        #uint16_t mag_cnt

        #OrientationReps body_to_imu
        #OrientationReps ltp_to_body

        #enum AhrsFCStatus status
        bool is_aligned


    AhrsFloatCmpl ahrs_fc

    void ahrs_fc_init()
    #void ahrs_fc_set_body_to_imu(OrientationReps *body_to_imu)
    void ahrs_fc_set_body_to_imu_quat(FloatQuat *q_b2i)
    void ahrs_fc_recompute_ltp_to_body()
    bool ahrs_fc_align(FloatRates *lp_gyro, FloatVect3 *lp_accel, FloatVect3 *lp_mag)
    void ahrs_fc_propagate(FloatRates *gyro, float dt)
    void ahrs_fc_update_accel(FloatVect3 *accel, float dt)
    void ahrs_fc_update_mag(FloatVect3 *mag, float dt)
    #void ahrs_fc_update_gps(struct GpsState *gps_s)

    void ahrs_fc_update_heading(float heading)
    void ahrs_fc_realign_heading(float heading)
