#include "../modell_factory.h"
#include "impl.h"
#include <algorithm>
#include <execution>
#include <random>
#include <functional>
#include <ranges>

void rejection_sampler::setData(simple_failure_times ftimes)
{
	this->failures_ = ftimes;
}

void rejection_sampler::setModell(imperfect_virtualage_likelihood modell)
{
	this->L_ = modell;
}

std::vector<double> rejection_sampler::solve(simulation_params param)
{
	std::default_random_engine gen;
	auto U = std::bind(std::uniform_real_distribution(0., 1.),gen);
	auto i = param.sample_size;
	L_.set_data(failures_);
	//sample
	for (; i-- > 0;)
	{
		sample_result tmp_;
		tmp_.accepted = false;
		tmp_.L = L_.get_likelihood();
		tmp_.params = L_.get_params();
		raw_result_.push_back(tmp_);
	}
	//get g(x) as const
	double L_max = std::max_element(std::execution::par,raw_result_.begin(), raw_result_.end(), [](const sample_result& A, const sample_result& B) {return A.L < B.L; })->L;
	//rejection sampling
	std::for_each(std::execution::par, raw_result_.begin(), raw_result_.end(), [&U, &L_max](sample_result& A) {
		auto u = U();
		if (u < A.L / L_max)
			A.accepted = true;
		else
			A.accepted = false;
		});
	//get estimation from result
	auto result = estimate_result_();
	return result;
}

std::vector<sample_result> rejection_sampler::get_raw_result()
{
	return raw_result_;
}

std::vector<double> rejection_sampler::estimate_result_()
{
	if (raw_result_.empty())
		throw std::exception("Internal error: raw_result is empty.");
	auto example = *raw_result_.begin();
	std::vector<double> avg(example.params.size(), 0);
	decltype(raw_result_) filtered_result_{};
//	for (sample_result I : raw_result_ | std::views::filter([](sample_result A) {return A.accepted; })) -- nice 2020 feature may use later
//		filtered_result_.push_back(I);
	for (auto I : raw_result_){
		if (I.accepted)
			filtered_result_.push_back(I);
	}
	auto accepted_number = filtered_result_.size();
	avg = std::transform_reduce(std::execution::par, filtered_result_.begin(), filtered_result_.end(), avg,
		[](const std::vector<double>& A, const std::vector<double>& B) {
			auto C = std::vector<double>(A.size(),0);
			std::transform(A.begin(), A.end(), B.begin(), C.begin(), std::plus<>());
			return C;
		}, [](sample_result& A) {return A.params; });
	std::for_each(avg.begin(), avg.end(), [accepted_number](double& param) {param = param / double(accepted_number); });
	return avg;
}
