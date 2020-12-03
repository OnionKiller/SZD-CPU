/**
* Collection of accespted tags by data, in an enum
*/
#pragma once

#include <string>

enum class tags
{
	Preventive,
	Repair,
	Complete,
	Incomplete
};

namespace tagmanager
{
	inline const std::string get_tag_name(tags from) {
		switch (from)
		{
		case tags::Preventive:
			return("Preventive");
			break;
		case tags::Repair:
			return("Repair");
			break;
		case tags::Complete:
			return("Complete");
			break;
		case tags::Incomplete:
			return("Incomplete");
			break;
		default:
			return("Undefined tag");
			throw(std::exception("Undefined tag found."));
			break;
		}
	}
}