#pragma once

#include <vector>
#include "data.h"


namespace helper_ 
{
	simple_failure F(double value, tags tag) {
		auto return_ = simple_failure(value);
		return_.add_tag(tag);
		return return_;
	};
}
namespace pdf04925834
{
	using namespace helper_;
	//beta=2.2,eta=1,a_r=0.8,a_p=0.3,weibull
	std::vector<simple_failure> list{
		 F(1.1873, tags::Preventive),
		 F(0.3650, tags::Repair),
		 F(0.0913, tags::Repair),
		 F(0.0183, tags::Preventive),
		 F(0.0535, tags::Repair),
		 F(0.0382, tags::Preventive),
		 F(0.0694, tags::Repair),
		 F(0.1840, tags::Preventive),
		 F(0.2164, tags::Repair),
		 F(0.1117, tags::Repair),
		 F(0.0939, tags::Preventive),
		 F(0.0665, tags::Preventive),
		 F(0.0082, tags::Repair),
		 F(0.4003, tags::Repair),
		 F(0.0313, tags::Repair),
		 F(0.0135, tags::Preventive),
		 F(0.2025, tags::Repair),
		 F(0.0570, tags::Repair),
		 F(0.0819, tags::Preventive),
		 F(0.0673, tags::Repair),
		 F(0.2273, tags::Repair),
		 F(0.0087, tags::Preventive),
		 F(0.0010, tags::Preventive),
		 F(0.0194, tags::Repair),
		 F(0.1404, tags::Preventive),
		 F(0.0119, tags::Preventive),
		 F(0.0450, tags::Repair),
		 F(0.0730, tags::Preventive),
		 F(0.2667, tags::Repair),
		 F(0.1858, tags::Preventive),
		 F(0.0850, tags::Repair),
		 F(0.2109, tags::Preventive),
		 F(0.0444, tags::Repair),
		 F(0.1073, tags::Repair),
		 F(0.0301, tags::Repair),
		 F(0.0492, tags::Repair),
		 F(0.0819, tags::Repair),
		 F(0.0402, tags::Preventive),
		 F(0.0705, tags::Preventive),
		 F(0.0927, tags::Preventive),
		 F(0.0547, tags::Repair),
		 F(0.0145, tags::Preventive),
		 F(0.0567, tags::Preventive),
		 F(0.0067, tags::Repair),
		 F(0.0845, tags::Preventive),
		 F(0.0367, tags::Repair),
		 F(0.0375, tags::Repair),
		 F(0.0102, tags::Preventive),
		 F(0.0425, tags::Repair),
		 F(0.0221, tags::Preventive),
		 F(0.0170, tags::Repair),
		 F(0.0200, tags::Repair),
		 F(0.0649, tags::Repair),
		 F(0.0437, tags::Preventive),
		 F(0.0735, tags::Repair),
		 F(0.0295, tags::Repair),
		 F(0.0580, tags::Repair),
		 F(0.0013, tags::Preventive),
		 F(0.0223, tags::Repair),
		 F(0.0643, tags::Repair),
		 F(0.0220, tags::Preventive),
		 F(0.0250, tags::Preventive),
		 F(0.0356, tags::Repair),
		 F(0.0271, tags::Preventive),
		 F(0.1279, tags::Preventive),
		 F(0.0553, tags::Repair),
		 F(0.0390, tags::Preventive),
		 F(0.0154, tags::Preventive),
		 F(0.0009, tags::Preventive),
		 F(0.0748, tags::Repair),
		 F(0.0442, tags::Preventive),
		 F(0.0660, tags::Repair),
		 F(0.0142, tags::Repair),
		 F(0.0144, tags::Repair),
		 F(0.0104, tags::Repair),
		 F(0.0013, tags::Preventive),
		 F(0.0213, tags::Preventive),
		 F(0.1643, tags::Repair),
		 F(0.0697, tags::Preventive),
		 F(0.0020, tags::Repair),
		 F(0.0051, tags::Preventive),
		 F(0.0246, tags::Preventive),
		 F(0.0674, tags::Preventive),
		 F(0.1290, tags::Preventive),
		 F(0.0280, tags::Preventive),
		 F(0.0134, tags::Repair),
		 F(0.0257, tags::Repair),
		 F(0.0043, tags::Repair),
		 F(0.0634, tags::Repair),
		 F(0.1333, tags::Preventive),
		 F(0.0180, tags::Preventive),
		 F(0.0092, tags::Repair),
		 F(0.0259, tags::Repair),
		 F(0.0041, tags::Repair),
		 F(0.1093, tags::Repair),
		 F(0.0266, tags::Repair),
		 F(0.1446, tags::Preventive),
		 F(0.0004, tags::Repair),
		 F(0.0628, tags::Repair),
		 F(0.0550, tags::Preventive)
	};
}
namespace matlab
{
	//beta=,eta=,a_r,a_p,weibull 
	std::vector<simple_failure> list_0{

	};
}

