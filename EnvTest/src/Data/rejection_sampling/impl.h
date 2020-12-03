#pragma once
// rejection samlpling method implementad based on: https://ieeexplore.ieee.org/document/4925834

#include "../modell_factory.h"
#include "../data.h"

//interface to rejection sampler
class rejection_sampler
{
public:
	void setData(simple_failure_times ftimes);
	void setModell(imperfect_virtualage_likelihood modell);
	//solution solve(simulation_params);
protected:
	simple_failure_times failures_;
	//implement modell here
};