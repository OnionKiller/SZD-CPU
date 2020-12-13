//simple class to represent likelihood functions

#pragma once
#include <vector>
#include "../Data/data.h"
#include <string>

//TODO template overhaul
//TODO params overhaul
//TODO document

template<size_t paramNumber>
struct conditional_likelihood_value
{
	long double L = 0;
	double params[paramNumber];
	const size_t param_size = paramNumber;
protected:
	virtual inline double get(const size_t i) { return params[i]; };
	virtual inline const void set(const size_t i, double value) { params[i] = value; };
};

class likelihood
{
public:
	likelihood() = default;
	virtual ~likelihood() = default;
	virtual void add_data(failure_times failure_list) = delete;
	virtual std::string print() { return "virtual likelihood\n"; };
};


template<size_t paramNumber>
class likelihood_getter_interface
{
public:
	virtual conditional_likelihood_value<paramNumber> get_likelihood(bool parralell = true) = 0;
};

//template<template<size_t>typename modell>
//constexpr bool check_L_interface()
//{
//	static_assert(std::invocable_r< /*TODO*/ >::value);
//}