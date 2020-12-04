#pragma once

// factoring all the modell implementations in one place
// adding an additional interface to implement mathematic modells, is a TODO, probably with semantic statistical functions, and functionalities

#include "function_param.h"
#include "likelihood.h"
#include <memory>

// TODO lear up implementation with helper functions
struct imperfect_virtualage_likelihood_conditional_value : conditional_likelihood_value<4>
{
	//BEARP
	//getter
	double beta();
	//setter, @return setted value
	double beta(double beta);
	//getter
	double eta();
	//setter, @return setted value
	double eta(double eta);
	//getter
	double ar();
	//setter, @return setted value
	double ar(double ar);
	//getter
	double ap();
	//setter, @return setted value
	double ap(double ap);
	//print functionality
	virtual std::string print();
};

typedef imperfect_virtualage_likelihood_conditional_value imp_vage_cvalue;


//TODO template overhaul
class imperfect_virtualage_likelihood : public likelihood<4>
{
public:
	imperfect_virtualage_likelihood() = default;
	imperfect_virtualage_likelihood(imperfect_virtualage_likelihood&) = default;
	virtual void set_data(simple_failure_times failure_list);
	/*calculate likelihood
	* not thread safe way to calculate likelihood
	* */
	virtual conditional_likelihood_value<4> get_likelihood(bool parralell = false) override;
	/*set parameter limits to initialise
	* lower limit first, higher limit second all time, 
	* beta-eta-ar-ap list
	*/
	void set_params_limits(std::vector<double> BERP_list);
protected:
	simple_failure_times failure_list_;
	std::shared_ptr<function_param<double>> beta;
	std::shared_ptr<function_param<double>> eta;
	std::shared_ptr<function_param<double>> ar ;
	std::shared_ptr<function_param<double>> ap ;
	bool init = false;
	const inline bool check_init_();
	const inline imp_vage_cvalue init_random_();
	double Vi_1(size_t i, double ar, double ap);
};
