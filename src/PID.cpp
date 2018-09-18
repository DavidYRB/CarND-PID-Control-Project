#include "PID.h"
#include <iostream>
#include <cmath>
using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd, bool isTwiddle) {
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
  this->isTwiddle = isTwiddle;

  p_error = 0;
  i_error = 0;
  d_error = 0;

  temp_p.push_back(Kp);
  temp_p.push_back(Ki);
  temp_p.push_back(Kd);

  dp.push_back(0.1 * Kp);
  dp.push_back(0.1 * Ki);
  dp.push_back(0.1 * Kd);

  parameter_index = 0;
  step_per_loop = 50;
  current_loop = 0;
  current_step = 1;

  RUN_STATE = true;
  TUNE_UP = true;
  curr_err = 0;
  total_step_per_lap = 0;
}

void PID::UpdateError(double cte) {
  p_error = cte;
  i_error += cte;
  d_error = cte - cte_pre;
  cte_pre = cte;
  double dp_sum = dp[0] + dp[1] + dp[2];

  if(isTwiddle && dp_sum > 0.001){
    std::cout << "I am I tuning? " << std::endl;
    std::cout << "Run state: " << RUN_STATE << " Tune state: " << TUNE_UP << std::endl;
    if(RUN_STATE){
      if(current_step < step_per_loop * 2){
        if(current_step > step_per_loop){
          curr_err += pow(cte, 2);
        }
        current_step++;
        std::cout << "Curr error: " << curr_err << " cte: " << cte << " index: " << parameter_index << std::endl;
        return;
      }
      total_step_per_lap += step_per_loop * 2;
      std::cout << "total step: " << total_step_per_lap << std::endl;
      RUN_STATE = false;
      current_step = 0;
      curr_err /= step_per_loop;
      current_loop++;
    }
    // When the first loop, just record the error as best error, and change 
    // one parameter to run again.
    std::cout << "Current Loop: " << current_loop << std::endl;
    if(current_loop == 1){
      best_err = curr_err;
      temp_p[parameter_index] += dp[parameter_index];
      RUN_STATE = true;
      curr_err = 0;
      return;
    }
    else{
      std::cout << "current error: " << curr_err << " best Error: " << best_err << std::endl;
      if(TUNE_UP){
        if(curr_err < best_err){
          // change the dp value of current index position
          dp[parameter_index] *= 1.1;
          best_err = curr_err;
          // change the index of parameter and make change to next parameter
          parameter_index++;
          parameter_index %= 3;
          temp_p[parameter_index] += dp[parameter_index];
        }
        else{
          // this condition means we need to tune downwards, and run again
          temp_p[parameter_index] -= 2 * dp[parameter_index];
          RUN_STATE = true;
          // change TUNE_UP flag to false means we tune parameter to another direciton
          TUNE_UP = false;
        }
        RUN_STATE = true;
        curr_err = 0;
        return;
      }
      
      else{
        if(curr_err < best_err){
          dp[parameter_index] *= 1.1;
          best_err = curr_err;
          // change the index of parameter and make change to next parameter
          parameter_index++;
          parameter_index %= 3;
          temp_p[parameter_index] += dp[parameter_index];
        }
        else{
          temp_p[parameter_index] += dp[parameter_index];
          dp[parameter_index] *= 0.9;
          parameter_index++;
          parameter_index %= 3;
          temp_p[parameter_index] += dp[parameter_index];
        }
        TUNE_UP = true;
        RUN_STATE = true;
        curr_err = 0;
        return;
      }
    }
  }
  if(dp_sum <= 0.001){
    std::cout << "Final answer: " << std::endl;
    std::cout << "Kp: " << temp_p[0] << " Ki: " << temp_p[1] << " Kd: " << temp_p[2] << std::endl;
  }
}

double PID::TotalError() {
  std::cout << "Kp: " << temp_p[0] << " Ki: " << temp_p[1] << " Kd: " << temp_p[2] << std::endl;
  return -temp_p[0] * p_error - temp_p[1] * i_error - temp_p[2] * d_error;

}
