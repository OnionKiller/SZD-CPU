//simple class to represent likelihood functions

#pragma once
#include <vector>
#include "data.h"

//TODO template overhaul
//TODO params overhaul
//TODO document
class likelihood
{
public:
	likelihood() = default;
	virtual ~likelihood() = default;
	virtual void add_data(failure_times failure_list) = delete;
	virtual double get_likelihood() = 0;
	virtual std::vector<double> get_params() { return last_values_; };
protected:
	std::vector<double> last_values_ = {};
};