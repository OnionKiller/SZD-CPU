#include "modell_factory.h"
#include <iostream>
#include <cmath>

void imperfect_virtualage_likelihood::add_data(simple_failure_times failure_list)
{
	simple_failure_times filtered;
	for (auto&& I : failure_list.get_failure_list())
	{
		if (I.has_tag(tags::Complete) && \
			(I.has_tag(tags::Preventive) ^ I.has_tag(tags::Repair))) {
			filtered.add_failure(I);
		}
		else
		{
			std::cout << "Data dropped: " << I.print() << std::endl;
		}
	}
	failure_list = filtered;
}

double imperfect_virtualage_likelihood::get_likelihood()
{
	if (!check_init_())
		throw std::exception("likelihood isn't initialised, add param borders");
	//implement https://ieeexplore.ieee.org/document/4925834
	init_random_();
	//create all Vi_x w/ info
	struct Vi {
		Vi(size_t i, double v, bool b) :i(i), value(v), is_repair(b) {};
		size_t i;
		double value;
		bool is_repair = false;
	};
	auto f_list = failure_list_.get_failure_list();
	auto Vi_list = std::vector<Vi>{ Vi(0,0,false)};
	//V_(i-1)+X_i list
	auto Vi_1px_list = std::vector<Vi>{};
	for (size_t i = 0; i < f_list.size(); ++i)
	{
		auto is_repair = f_list[i].has_tag(tags::Repair);
		auto value_with_next = Vi_list.rbegin()->value + f_list[i].get_failure_time();
		auto value = Vi_list.rbegin()->value + (is_repair? Car:Cap )*f_list[i].get_failure_time();
		Vi_list.push_back(Vi(i,value,is_repair));
		Vi_1px_list.push_back(Vi(i, value_with_next, is_repair));
	}
	//agregate
	//fist part
	auto Vi_save = Vi_1px_list;
	auto pi = std::transform_reduce(std::execution::par, Vi_1px_list.begin(), Vi_1px_list.end(),double(0), [](double a, double b) {return a * b; }, [this](Vi& A) {
		if (!A.is_repair)
			return 1;
		auto value = std::powl(A.value, (Cbeta - 1)) * Cbeta / (std::powl(Ceta, Cbeta));
		});
	auto sum = std::transform_reduce(std::execution::par, Vi_save.begin(), Vi_save.end(), Vi_list.begin(), double(0), [](double a, double b) {return a + b; }, [this](Vi& Vix, Vi& Vi) {
		return std::powl(Vi.value, Cbeta) - std::powl(Vix.value, Cbeta);
		});
	auto result = pi * std::expl(1. / std::powl(Ceta, Cbeta) * sum);
	return result;
}

void imperfect_virtualage_likelihood::set_params_limits(std::vector<double> BERP_list)
{
	if (BERP_list.size() != 8)
		throw std::exception("BERP size is wrong");
	this->beta = new function_param(BERP_list[0], BERP_list[1]);
	this->eta = new function_param(BERP_list[2], BERP_list[3]);
	this->ar = new function_param(BERP_list[4], BERP_list[5]);
	this->ap = new function_param(BERP_list[6], BERP_list[7]);
	last_values_.resize(4);
	init_random_();
	init = true;
}

imperfect_virtualage_likelihood::~imperfect_virtualage_likelihood()
{
	if (this->beta != nullptr)
		delete beta;
	if (this->eta != nullptr)
		delete eta;
	if (this->ar != nullptr)
		delete ar;
	if (this->ap != nullptr)
		delete ap;
}

inline const bool imperfect_virtualage_likelihood::check_init_()
{
	return init && !failure_list_.empty();
}

inline const void imperfect_virtualage_likelihood::init_random_()
{
	//*cry emoji*😢
	last_values_[0]  = beta->get_random_variable();
	Cbeta = last_values_[0];
	last_values_[1] = Ceta = eta->get_random_variable();
	Ceta = last_values_[1];
	last_values_[2] = Car =  ar->get_random_variable();
	Car = last_values_[2];
	last_values_[3] = Cap = ap->get_random_variable();
	Cap = last_values_[3];
	return void();
}

//TODO alternatuive option to sequentially init Vi_1
double imperfect_virtualage_likelihood::Vi_1(size_t i)
{
	auto end = failure_list_.begin() + i-1;
	if (i < 0)
		throw std::exception("Bad index");
	auto r = std::transform_reduce(std::execution::par, failure_list_.begin(), end, double(0.), \
		[](double a, double b)->double {return a + b; }, [this](simple_failure A) {
			if(A.has_tag(tags::Repair))
				return A.get_failure_time()*Cap; 
			else
				return A.get_failure_time() * Car;
		});
	return r;
}
