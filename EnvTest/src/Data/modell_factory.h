#pragma once

// factoring all the modell implementations in one place
// adding an additional interface to implement mathematic modells, is a TODO, probably with semantic statistical functions, and functionalities

#include "function_param.h"
#include "likelihood.h"

//TODO templete overhaul
//TODO memory overhaul
class imperfect_virtualage_likelihood : public likelihood
{
public:
	virtual void add_data(simple_failure_times failure_list);
	//calculate likelihood
	virtual double get_likelihood() override;
	/*set parameter limits to initialise
	* lower limit first, higher limit second all time, 
	* beta-eta-ar-ap list
	*/
	void set_params_limits(std::vector<double> BERP_list);
	//release param pointers
	virtual ~imperfect_virtualage_likelihood();
protected:
	simple_failure_times failure_list_;
	function_param<double>* beta = nullptr;
	function_param<double>* eta = nullptr;
	function_param<double>* ar = nullptr;
	function_param<double>* ap = nullptr;
	double Cbeta = 0, Ceta = 0, Car = 0, Cap = 0;
	bool init = false;
	const inline bool check_init_();
	const inline void init_random_();
	double Vi_1(size_t i);
};
