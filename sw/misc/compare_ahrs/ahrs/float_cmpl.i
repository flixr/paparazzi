/* File : float_cmpl.i */
%module float_cmpl
%feature("autodoc", "3");
%include "typemaps.i"

/* apply all of the bool typemaps to the paparazzi bool_t */
%apply bool { bool_t };

%include pprz_algebra_float.i

%{
#define SWIG_FILE_WITH_INIT
#include "subsystems/ahrs/ahrs_float_cmpl.h"
%}

/* don't wrap everything in header
%include "subsystems/ahrs/ahrs_float_cmpl.h"
* instead only wrap needed stuff
*/

struct AhrsFloatCmpl {
  struct FloatRates gyro_bias;
  struct FloatRates rate_correction;
  struct FloatRates imu_rate;
  struct FloatQuat ltp_to_imu_quat;
  struct FloatRMat ltp_to_imu_rmat;

  bool_t correct_gravity; ///< enable gravity correction during coordinated turns
  float ltp_vel_norm; ///< velocity norm for gravity correction during coordinated turns
  bool_t ltp_vel_norm_valid;

  float accel_omega;  ///< filter cut-off frequency for correcting the attitude from accels (pseudo-gravity measurement)
  float accel_zeta;   ///< filter damping for correcting the gyro-bias from accels (pseudo-gravity measurement)
  float mag_omega;    ///< filter cut-off frequency for correcting the attitude (heading) from magnetometer
  float mag_zeta;     ///< filter damping for correcting the gyro bias from magnetometer

  /** sets how strongly the gravity heuristic reduces accel correction.
   * Set to zero in order to disable gravity heuristic.
   */
  uint8_t gravity_heuristic_factor;
  float weight;

  bool_t heading_aligned;
  struct FloatVect3 mag_h;

  /* internal counters for the gains */
  uint16_t accel_cnt; ///< number of propagations since last accel update
  uint16_t mag_cnt;   ///< number of propagations since last mag update

  struct OrientationReps body_to_imu;
  struct OrientationReps ltp_to_body;

  enum AhrsFCStatus status;
  bool_t is_aligned;
};

/*
extern struct AhrsFloatCmpl ahrs_fc;

extern void ahrs_fc_init(void);
extern void ahrs_fc_set_body_to_imu(struct OrientationReps *body_to_imu);
extern void ahrs_fc_set_body_to_imu_quat(struct FloatQuat *q_b2i);
extern void ahrs_fc_recompute_ltp_to_body(void);
extern bool_t ahrs_fc_align(struct FloatRates *lp_gyro, struct FloatVect3 *lp_accel,
                            struct FloatVect3 *lp_mag);
extern void ahrs_fc_propagate(struct FloatRates *gyro, float dt);
extern void ahrs_fc_update_accel(struct FloatVect3 *accel, float dt);
extern void ahrs_fc_update_mag(struct FloatVect3 *mag, float dt);
extern void ahrs_fc_update_gps(struct GpsState *gps_s);
void ahrs_fc_update_heading(float heading);
void ahrs_fc_realign_heading(float heading);
*/

%extend AhrsFloatCmpl {
  AhrsFloatCmpl() {
    ahrs_fc_init();
    return &ahrs_fc;
  }
  void propagate(struct FloatRates *gyro, float dt) {
    ahrs_fc_propagate(gyro, dt);
  }
  void update_mag(struct FloatVect3 *mag, float dt) {
    ahrs_fc_update_mag(mag, dt);
  }
  void update_accel(struct FloatVect3 *accel, float dt) {
    ahrs_fc_update_accel(accel, dt);
  }
  void recompute_ltp_to_body() {
    ahrs_fc_recompute_ltp_to_body();
  }
  void set_body_to_imu(struct FloatQuat *q_b2i) {
    ahrs_fc_set_body_to_imu_quat(q_b2i);
  }
  bool_t align(struct FloatRates *lp_gyro, struct FloatVect3 *lp_accel, struct FloatVect3 *lp_mag) {
    return ahrs_fc_align(lp_gyro, lp_accel, lp_mag);
  }
};
