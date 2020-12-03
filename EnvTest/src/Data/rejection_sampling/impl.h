#pragma once
// rejection samlpling method implementad based on: https://ieeexplore.ieee.org/document/4925834

#include "../modell_factory.h"
#include "../data.h"
#include <vector>

struct simulation_params
{
	size_t sample_size;
};

struct sample_result {
	bool accepted = false;
	std::vector<double> params;
	double L;
};

//interface to rejection sampler
class rejection_sampler
{
public:
	void setData(simple_failure_times ftimes);
	void setModell(imperfect_virtualage_likelihood modell);
	std::vector<double> solve(simulation_params params); 
	std::vector<sample_result> get_raw_result();
protected:
	simple_failure_times failures_;
	imperfect_virtualage_likelihood L_;
	//implement modell here
	std::vector<sample_result> raw_result_;
	std::vector<double> estimet_result_();
};