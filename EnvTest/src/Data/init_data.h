#pragma once

#include "data.h"
#include "raw_data.h"

inline simple_failure_times init_failure_datas()
{
	auto return_ = simple_failure_times();
	for (auto I : pdf04925834::list)
	{
		return_.add_failure(I);
	}
	return return_;
};