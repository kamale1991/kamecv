#include <iostream>
#include <vector>
#include <string>

#ifndef BAYSE_STOCHASTIC_MODEL_H_
#define BAYSE_STOCHASTIC_MODEL_H_

namespace Bayse
{
	enum StochasticModelType
	{
		LinearRegression = 0,
	};

	std::string model_name(int model_number)
	{
		if (model_number == 0)
			return "LinearRegression";
	};

	class StochasticModel
	{
	private:
		std::vector<double> dist_;
		const StochasticModelType model_type_;

	public:
		StochasticModel(StochasticModelType model_type) : model_type_(model_type){};
		~StochasticModel(){};

		void initialize();
	};
} // namespace Bayse

#endif