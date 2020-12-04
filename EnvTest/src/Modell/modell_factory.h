#pragma once

// factoring all the modell implementations in one place
// adding an additional interface to implement mathematic modells, is a TODO, probably with semantic statistical functions, and functionalities

#include "function_param.h"
#include "likelihood.h"
#include <memory>

//TODO templete overhaul
//TODO memory overhaul
class imperfect_virtualage_likelihood : public likelihood
{
public:
	imperfect_virtualage_likelihood() = default;
	imperfect_virtualage_likelihood(imperfect_virtualage_likelihood&) = default;
	virtual void set_data(simple_failure_times failure_list);
	/*calculate likelihood
	* not thread safe way to calculate likelihood
	* */
	virtual double get_likelihood() override;
	/*calculate likelihood in a thread safe fashion
	* helper return type
	*/
	struct th_likelihood{
		long double L = 0;
		double params[4];
	};
	/*calculate likelihood in a thread safe fashion
	*/
	virtual th_likelihood get_likelihood_th_safe();
	/*set parameter limits to initialise
	* lower limit first, higher limit second all time, 
	* beta-eta-ar-ap list
	*/
	void set_params_limits(std::vector<double> BERP_list);
	//release param pointers
	virtual ~imperfect_virtualage_likelihood();
protected:
	simple_failure_times failure_list_;
	std::shared_ptr<function_param<double>> beta;
	std::shared_ptr<function_param<double>> eta;
	std::shared_ptr<function_param<double>> ar ;
	std::shared_ptr<function_param<double>> ap ;
	double Cbeta = 0, Ceta = 0, Car = 0, Cap = 0;
	bool init = false;
	const inline bool check_init_();
	const inline void init_random_();
	double Vi_1(size_t i);
	//params are BERP
	long double thread_safe_likelihood_(double params[4]);
};
