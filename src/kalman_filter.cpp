#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_;
  P_ = F_ * P_ * F_.transpose() + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Kalman Filter equations
  */
    MatrixXd Ht = H_.transpose();
  VectorXd y = z -  H_ * x_;
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd K = P_ * Ht * S.inverse();

  x_ = x_ + K*y;
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K*H_)*P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
  TODO:
    * update the state by using Extended Kalman Filter equations
  */

  double px = x_(0);
  double py = x_(1);
  double vx = x_(2);
  double vy = x_(3);
  VectorXd z_pred(3);
    double rho = sqrt(px*px + py*py); // sqrt(x_(0)* x_(0) + x_(1)* x_(1));
    double arctan = atan2(py, px); // atan2(x_(1), x_(0));
    double rho_dot = (px * vx + py * vy) / std::max(rho, 0.0001);;

  z_pred<<rho, arctan, rho_dot;

  // norm arctan
  //double P2 = 2*M_PI;
  //z_pred(1) = z_pred(1) - floor( z_pred(1)/ P2) * P2 - M_PI;

    MatrixXd Ht = H_.transpose();
  VectorXd y = z -  z_pred;
    y(1) = atan2(sin(y(1)), cos(y(1)));
    std::cout<<y(1)<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<std::endl;
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd K = P_ * Ht * S.inverse();

  x_ = x_ + K*y;
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K*H_)*P_;

}
