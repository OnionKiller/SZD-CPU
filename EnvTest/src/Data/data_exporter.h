#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../Modell/rejection_sampling/full_impl.h"

inline void export_data( const std::vector<sample_result> raw_data,const std::string filename)
{
	std::fstream file(filename, std::fstream::trunc | std::fstream::out);
	file << "layout:" << std::endl;
	file << "accepted,L_value";
	for (auto i = 0; i < raw_data[0].params.size(); i++)
		file << ",param[" << i << ']';
	file << std::endl;

	for (auto&& I : raw_data)
	{
		file << static_cast<int>(I.accepted) << ',' << I.L;
		for (auto&& J : I.params)
			file << ',' << J;
		file << std::endl;
	}
	file.close();
}