#include "modell_factory.h"
#include <iostream>
#include <cmath>

double imperfect_virtualage_likelihood_conditional_value::beta()
{
	return params[0];
}

double imperfect_virtualage_likelihood_conditional_value::beta(double beta)
{
	params[0] = beta;
	return params[0];
}

double imperfect_virtualage_likelihood_conditional_value::eta()
{
	return params[1];
}

double imperfect_virtualage_likelihood_conditional_value::eta(double eta)
{
	params[1] = eta;
	return params[1];
}

double imperfect_virtualage_likelihood_conditional_value::ar()
{
	return params[2];
}

double imperfect_virtualage_likelihood_conditional_value::ar(double ar)
{
	params[2] = ar;
	return params[2];
}

double imperfect_virtualage_likelihood_conditional_value::ap()
{
	return params[3];
}

double imperfect_virtualage_likelihood_conditional_value::ap(double ap)
{
	params[3] = ap;
	return params[3];
}

void imperfect_virtualage_likelihood::set_data(simple_failure_times failure_list)
{
	simple_failure_times filtered;
	for (auto&& I : failure_list.get_failure_list())
	{
		if (I.has_tag(tags::Complete) && \
			(I.has_tag(tags::Preventive) ^ I.has_tag(tags::Repair))) 
		{
			filtered.add_failure(I);
		}
		else
		{
			std::cout << "Data dropped: " << I.print();
		}
	}
	this->failure_list_ = filtered;
}

//TODO implement parallelisation switch
conditional_likelihood_value<4> imperfect_virtualage_likelihood::get_likelihood(bool parralell)
{

	if (!check_init_())
		throw std::exception("likelihood isn't initialised, add param borders");
	//get initial state
	auto I = init_random_();
	//implement https://ieeexplore.ieee.org/document/4925834
	//create all Vi_x w/ info
	struct Vi {
		Vi(size_t i, long double v, bool b) :i(i), value(v), is_repair(b) {};
		size_t i;
		long double value;
		bool is_repair = false;
	};
	auto f_list = failure_list_.get_failure_list();
	auto Vi_list = std::vector<Vi>{ Vi(0,0,false) };
	//V_(i-1)+X_i list
	auto Vi_1px_list = std::vector<Vi>{};
	for (size_t i = 0; i < f_list.size(); ++i)
	{
		auto is_repair = f_list[i].has_tag(tags::Repair);
		auto value_with_next = Vi_list.rbegin()->value + f_list[i].get_failure_time();
		auto value = Vi_list.rbegin()->value + (is_repair ? I.ar() : I.ap()) * f_list[i].get_failure_time();
		Vi_list.push_back(Vi(i, value, is_repair));
		Vi_1px_list.push_back(Vi(i, value_with_next, is_repair));
	}
	//agregate
	//fist part
	auto Vi_save = Vi_1px_list;
	auto pi = std::transform_reduce(/*std::execution::par,*/
		Vi_1px_list.begin(), Vi_1px_list.end(), (long double)1., std::multiplies<>(), [&](Vi& A)->long double {
			if (!A.is_repair)
				return 1.;
			long double value = std::powl(A.value, (I.beta() - 1)) * I.eta() / (std::powl(I.eta(), I.beta()));
			return value;
		});
	auto sum = std::transform_reduce(/*std::execution::par,*/ 
		Vi_save.begin(), Vi_save.end(), Vi_list.begin(), (long double)0, std::plus<>(), [&](Vi& Vix, Vi& Vi)->long double {
			return std::powl(Vi.value, I.beta()) - std::powl(Vix.value, I.beta());
		});
	auto result = pi * std::expl(1. / std::powl(I.eta(), I.beta()) * sum);
	I.L = result;
	return conditional_likelihood_value<4>(I);
}

void imperfect_virtualage_likelihood::set_params_limits(std::vector<double> BERP_list)
{
	if (BERP_list.size() != 8)
		throw std::exception("BERP size is wrong");
	this->beta = std::make_shared<function_param<double>>(BERP_list[0], BERP_list[1]);
	this->eta  = std::make_shared<function_param<double>>(BERP_list[2], BERP_list[3]);
	this->ar   = std::make_shared<function_param<double>>(BERP_list[4], BERP_list[5]);
	this->ap   = std::make_shared<function_param<double>>(BERP_list[6], BERP_list[7]);
	init = true;
}

inline const bool imperfect_virtualage_likelihood::check_init_()
{
	return init && !failure_list_.empty();
}

inline const imp_vage_cvalue imperfect_virtualage_likelihood::init_random_()
{
	auto return_ = imp_vage_cvalue();
	return_.beta(beta->get_random_variable());
	return_.eta(eta->get_random_variable());
	return_.ar(ar->get_random_variable());
	return_.ap(ap->get_random_variable());
	return return_;
}

double imperfect_virtualage_likelihood::Vi_1(size_t i, double ar, double ap)
{
	auto end = failure_list_.begin() + i-1;
	if (i < 0)
		throw std::exception("Bad index");
	auto r = std::transform_reduce(std::execution::par, failure_list_.begin(), end, double(0.), 
		std::plus<>(), [&ar,&ap](simple_failure A) {
			if(A.has_tag(tags::Repair))
				return A.get_failure_time() * ap;
			else
				return A.get_failure_time() * ar;
		});
	return r;
}

