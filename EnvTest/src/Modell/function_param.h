//simple implementation for function parameter simulation
#pragma once
#include <type_traits>
#include <random>
#include <functional>
#include <typeinfo>
#include <chrono>

template<typename dataType>
class function_param
{
public:
	typedef std::conditional_t<
		std::is_integral_v<dataType>,
		std::uniform_int_distribution<dataType>,
			std::conditional_t<
			std::is_floating_point_v<dataType>,
			std::uniform_real_distribution<dataType>,
			void>
		> distributionType;
	function_param() = delete;
	function_param(dataType min, dataType max) :min(min), max(max) {
		//seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		//generator = std::default_random_engine(seed);
		std::random_device g;
		generator = std::default_random_engine(g());
		distribution = distributionType(min, max);
	};
	function_param(function_param& C)
	{
		min = C->min;
		max = C->max;
		distribution = distributionType(min, max);
		generator = C->generator;
	}
	//TODO add fallback if no type is accessible #SFINALEhell
	//function_param(dataType min, dataType max) :min(min), max(max) {
	//	static_assert(std::is_floating_point_v<dataType> || std::is_integral_v<dataType>, \
	//		"Can't generate random value from non arithmetic values.");
	//}
	dataType get_random_variable() {
		auto dVal = distribution(generator);
		return dVal;
	}

protected:
	dataType min;
	dataType max;
	std::default_random_engine generator;
	distributionType distribution;
};

template<typename dataType>
class function_param_tests : public function_param<dataType>
{
public:
	function_param_tests() = delete;
	function_param_tests(dataType min, dataType max) :function_param<dataType>(min,max) {
		std::random_device g;
		randomised_generator = std::default_random_engine(g());
	};
	//FOR FUN ONLY NOT TESTED
	auto binded_gen0() { return this->distribution(random_generator); };
	auto binded_gen1() { return this->distribution(this->generator); };
	auto binded_gen2() { return this->distribution(randomised_generator); };
protected:
	std::random_device random_generator;
	std::default_random_engine randomised_generator;
};