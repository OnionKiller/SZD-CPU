//simple implementation for function parameter simulation
#pragma once
#include <type_traits>
#include <random>
#include <functional>
#include <typeinfo>

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
		distribution = distributionType(min, max);
	};
	function_param(function_param& C)
	{
		min = C->min;
		max = C->max;
		distribution = distributionType(min, max);
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
private:
	dataType min;
	dataType max;
	std::random_device generator;
	distributionType distribution;
	//FOR FUN ONLY NOT TESTED
	auto binded_gen()-> decltype(distribution(generator)) { return distribution(generator); };
};
