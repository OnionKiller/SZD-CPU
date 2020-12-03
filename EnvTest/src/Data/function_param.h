//simple implementation for function parameter simulation
#pragma once
#include <type_traits>
#include <random>
#include <functional>

template<typename dataType>
class function_param
{
public:
	function_param() = delete;
	template <typename T = dataType, std::enable_if_t < std::is_integral < T >::value, bool > = true >
	function_param(dataType min, dataType max) :min(min), max(max) {
		std::uniform_int_distribution<dataType> distribution(min, max);
		random_gen = [&, distribution]()->dataType {return distribution(this->generator); };
	};
	template <typename T = dataType, std::enable_if_t < std::is_floating_point < T >::value, bool > = true >
	function_param(dataType min, dataType max) :min(min), max(max) {
		std::uniform_real_distribution<dataType> distribution(min, max);
		random_gen = [&, distribution]()->dataType {return distribution(this->generator); };
	};
	//TODO add fallback if no type is accessible #SFINALEhell
	//function_param(dataType min, dataType max) :min(min), max(max) {
	//	static_assert(std::is_floating_point_v<dataType> || std::is_integral_v<dataType>, \
	//		"Can't generate random value from non arithmetic values.");
	//}
	dataType get_random_variable() {
		auto dVal = random_gen();
		return dVal;
	}
private:
	dataType min;
	dataType max;
	std::default_random_engine generator;
	std::function<dataType()> random_gen;
};
