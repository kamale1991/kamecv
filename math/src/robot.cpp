#include "../include/robot.h"
#include <cmath>

namespace Bayse
{
	std::ostream &operator<<(std::ostream &os, const State &d)
	{
		os << d.x_m << " " << d.y_m << " " << d.theta;
		return os;
	}

	void Robot::move(ControlCommand &control)
	{
		double v = control.v_m_s_;

		double omega = control.omega_rad_s_;
		if (omega == 0.)
		{
			x_m_ += v * std::cos(theta_rad_);
			y_m_ += v * std::sin(theta_rad_);
			return;
		}

		double inv_omega = control.inv_omega_s_rad_;
		double delta_t_s = control.delta_t_s_;

		x_m_ += v * inv_omega * (std::sin(theta_rad_ + omega * delta_t_s) - std::sin(theta_rad_));
		y_m_ += v * inv_omega * (-std::cos(theta_rad_ + omega * delta_t_s) + std::cos(theta_rad_));
		theta_rad_ += omega * delta_t_s;
		return;
	};

	void Robot::report_current_position()
	{
		std::cout << "x=" << x_m_ << " y=" << y_m_ << " theta=" << theta_rad_ << std::endl;
	}

	State Robot::get_current_state()
	{
		return State(x_m_, y_m_, theta_rad_);
	};

	State state_transition(double v, double omega, double time, double x, double y, double theta)
	{
		ControlCommand control(time, v, omega);
		Robot robot(x, y, theta);
		robot.move(control);
		return robot.get_current_state();
	};

} // namespace Bayse
