#ifndef PID_H
#define PID_H
#include <vector>

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double cte_pre;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;

  // parameters for twiddle methods to tune PID parameters
  std::vector<double> temp_p;
  std::vector<double> dp;
  double best_err;
  int step_per_loop;
  int current_loop;
  int current_step;
  bool isTwiddle;
  int parameter_index;

  bool RUN_STATE;
  bool TUNE_UP;
  double curr_err;
  double total_step_per_lap;


  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd, bool isTwiddle);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  /*
  * Use twiddle method to tune the parameter
  */
  

};

#endif /* PID_H */
