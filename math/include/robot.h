#include <iostream>
#include <vector>
#include <string>

#ifndef BAYSE_ROBOT_H_
#define BAYSE_ROBOT_H_

namespace Bayse
{

	class Coordinate
	{
	private:
		const double space_unit_;
		const double time_unit_;

	public:
		Coordinate(double space_unit, double time_unit) : space_unit_(space_unit), time_unit_(time_unit){};
		~Coordinate(){};
	};

	struct State
	{
		double x_m;
		double y_m;
		double theta;
		State(double x_m_, double y_m_, double theta_) : x_m(x_m_), y_m(y_m_), theta(theta_){};
		State(){};

		bool operator==(const State &other) const
		{
			if (x_m == other.x_m && y_m == other.y_m && theta == other.theta)
			{
				return true;
			}
			return false;
		}
	};

	std::ostream &operator<<(std::ostream &os, const State &d);

	class ControlCommand
	{
	public:
		const double delta_t_s_;
		const double v_m_s_;
		const double omega_rad_s_;
		double inv_omega_s_rad_;
		ControlCommand(double delta_t_s, double v_m_s, double omega_rad_s) : delta_t_s_(delta_t_s), v_m_s_(v_m_s), omega_rad_s_(omega_rad_s)
		{
			inv_omega_s_rad_ = 1.0 / omega_rad_s_;
		};
		~ControlCommand(){};
	};

	class Robot
	{
	private:
		uint64_t current_time_s_;
		// ロボットの姿勢, 状態空間
		// 現在の位置、更新の度に変数を書き換えていく
		double x_m_;
		double y_m_;
		double theta_rad_;

	public:
		Robot(double x_m, double y_m, double theta_rad) : x_m_(x_m), y_m_(y_m), theta_rad_(theta_rad){};
		~Robot(){};
		void move(ControlCommand &control);
		void report_current_position();
		State get_current_state();
	};

	State state_transition(double v, double omega, double time, double x, double y, double theta);

	class Agent
	{
	private:
		ControlCommand control_;

	public:
		Agent(ControlCommand control) : control_(control){};
		~Agent(){};
		ControlCommand decision()
		{
			return control_;
		};
	};

} // namespace Bayse

#endif