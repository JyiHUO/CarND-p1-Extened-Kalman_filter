#include <iostream>
#include "tools.h"

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
  TODO:
    * Calculate the RMSE here.
  */
    VectorXd rmse(4);
    rmse << 0,0,0,0;

    if (estimations.size() != ground_truth.size() || estimations.size()==0){
        cout << "Invalid estimation or ground_truth data" << endl;
        return rmse;
    }

    for(int i=0;i<estimations.size();i++){
        VectorXd res = estimations[i] - ground_truth[i];
        res = res.array() * res.array();
        rmse += res;
    }
    rmse = rmse/estimations.size();
    rmse = rmse.array().sqrt();
    // cout<< "rmse: "<<rmse<<endl;
    return rmse;

}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
  TODO:
    * Calculate a Jacobian here.
  */
    double px = x_state(0);
    double py = x_state(1);
    double vx = x_state(2);
    double vy = x_state(3);

    //pre-compute a set of terms to avoid repeated calculation
    double c1 = px*px+py*py;
    double c2 = sqrt(c1);
    double c3 = (c1*c2);

    MatrixXd H_ = MatrixXd(3,4);
    H_<<0,0,0,0,
        0,0,0,0,
        0,0,0,0;
    if(fabs(c1) < 0.0001) {
        std::cout << "CalculateJacobian () - Error - Division by Zero" << std::endl;
        return H_;
    }

    //compute the Jacobian matrix
    H_ << (px/c2), (py/c2), 0, 0,
            -(py/c1), (px/c1), 0, 0,
            py*(vx*py - vy*px)/c3, px*(px*vy - py*vx)/c3, px/c2, py/c2;
    return H_;
}
