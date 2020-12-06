#include "pch.h"
#include "../EnvTest/src/Data/data.h"
#include "../EnvTest/src/Modell/modell_factory.h"
#include "../EnvTest/src/Modell/rejection_sampling/impl.h"


//time assertion (https://github.com/google/googletest/issues/348#issuecomment-492810513)
#include <future>
#include <chrono>
#define ASSERT_DURATION_LE(microsecs, stmt) { \
  std::promise<bool> completed; \
  auto stmt_future = completed.get_future(); \
  std::thread([&](std::promise<bool>& completed) { \
    stmt; \
    completed.set_value(true); \
  }, std::ref(completed)).detach(); \
  if(stmt_future.wait_for(std::chrono::microseconds(microsecs)) == std::future_status::timeout) \
    GTEST_FATAL_FAILURE_("       timed out (> " #microsecs \
    " microseconds). Check code for infinite loops"); \
}

namespace data_tests {
	class tagger_test : public tagged_failure
	{
		virtual double get_failure_time() { return 0; };
	};
	std::vector<tags> test_case{ tags::Complete, tags::Incomplete, tags::Preventive, tags::Repair };
	TEST(tagmanager, tag_prints)
	{
		ASSERT_EQ(tagmanager::get_tag_name(tags::Complete), "Complete") << "typoo";
		ASSERT_EQ(tagmanager::get_tag_name(tags::Incomplete), "Incomplete") << "typoo";
		ASSERT_EQ(tagmanager::get_tag_name(tags::Repair), "Repair") << "typoo";
		ASSERT_EQ(tagmanager::get_tag_name(tags::Preventive), "Preventive") << "typoo";
	}
	TEST(tagged_failure, init) {
		EXPECT_NO_THROW(auto I = tagger_test());
	}
	TEST(tagged_failure, addtag) {
		auto I = tagger_test();
		ASSERT_NO_THROW(I.add_tag(tags::Complete));
		ASSERT_TRUE(I.has_tag(tags::Complete));
	}
	TEST(tagged_failure, addAlltags)
	{
		auto I = tagger_test();
		EXPECT_NO_THROW(I.add_tag(test_case));
		ASSERT_TRUE(I.has_tag(tags::Complete));
		ASSERT_TRUE(I.has_tag(tags::Incomplete));
		ASSERT_TRUE(I.has_tag(tags::Repair));
		ASSERT_TRUE(I.has_tag(tags::Preventive));
	}
	TEST(tagged_failure, addDuplicated)
	{
		auto I = tagger_test();
		I.add_tag(tags::Complete);
		ASSERT_NO_THROW(I.add_tag(tags::Complete));
		ASSERT_TRUE(I.has_tag(tags::Complete));
	}
	TEST(tagged_failure, removeTag)
	{
		auto I = tagger_test();
		ASSERT_NO_THROW(I.add_tag(tags::Complete));
		ASSERT_NO_THROW(I.remove_tag(tags::Complete));
		ASSERT_FALSE(I.has_tag(tags::Complete));
	}
	TEST(tagged_failure, removeAllTags)
	{
		auto I = tagger_test();
		EXPECT_NO_THROW(I.add_tag(test_case));
		ASSERT_NO_THROW(I.remove_tag(test_case));
		ASSERT_FALSE(I.has_tag(tags::Complete));
		ASSERT_FALSE(I.has_tag(tags::Incomplete));
		ASSERT_FALSE(I.has_tag(tags::Repair));
		ASSERT_FALSE(I.has_tag(tags::Preventive));
	}
	TEST(tagged_failure, removeNonExistentTag)
	{
		auto I = tagger_test();
		ASSERT_NO_THROW(I.remove_tag(tags::Complete));
		ASSERT_FALSE(I.has_tag(tags::Complete));
	}
	TEST(tagged_failure, print1Tag)
	{
		auto I = tagger_test();
		I.add_tag(tags::Complete);
		std::string s;
		EXPECT_NO_THROW(s = I.print(););
		ASSERT_EQ(s, "[Complete]\n");
	}
	TEST(tagged_failure, printAllTag)
	{
		auto I = tagger_test();
		I.add_tag(test_case);
		std::string s;
		EXPECT_NO_THROW(s = I.print(););
		ASSERT_EQ(s, "[Complete,Incomplete,Preventive,Repair]\n");
	}
	TEST(simple_failure, addFailureTime)
	{
		ASSERT_NO_THROW(auto I = simple_failure(100.));
	}
	TEST(simple_failure, getFailureTime)
	{
		auto I = simple_failure(100.);
		ASSERT_EQ(I.get_failure_time(), 100.);
	}
	TEST(simple_failure, print1Tag)
	{
		auto I = simple_failure(1.2);
		I.add_tag(tags::Complete);
		std::string s;
		EXPECT_NO_THROW(s = I.print(););
		ASSERT_EQ(s, "{ x = 1.200000 ; [Complete]}\n");
	}
	TEST(simple_failure, printAllTag)
	{
		auto I = simple_failure(1.0);
		I.add_tag(test_case);
		std::string s;
		EXPECT_NO_THROW(s = I.print(););
		ASSERT_EQ(s, "{ x = 1.000000 ; [Complete,Incomplete,Preventive,Repair]}\n");
	}
};
namespace modell_test {
	TEST(function_param, init)
	{
		ASSERT_NO_THROW(auto I = function_param<int>(0, 10););
		ASSERT_NO_THROW(auto I = function_param<long int>(0, 10););
		ASSERT_NO_THROW(auto I = function_param<double>(0., 10.););
		ASSERT_NO_THROW(auto I = function_param<long double>(0., 10.););
	}
	TEST(function_param, generatei)
	{
		auto I = function_param<int>(0, 1);
		auto v = I.get_random_variable();
		ASSERT_TRUE(0 <= v && v <= 1) << "Bad value: " << v;
	}
	TEST(function_param, generated)
	{
		auto I = function_param<double>(0., 1.);
		auto v = I.get_random_variable();
		ASSERT_TRUE(0. <= v && v <= 1.) << "Bad value: " << v;
	}
	TEST(function_param, generate_valuetest)
	{
		auto I = function_param<double>(0., 1.);
		for (auto i = 100; i-- > 0;) {
			auto v = I.get_random_variable();
			ASSERT_TRUE(0. <= v && v < 1.) << "Bad value: " << v;
		}
	}
	TEST(function_param, generate_perftest)
	{
		constexpr int TEST_SAMPLE_SIZE =  100;
		auto I = function_param<double>(0., 1.);
		volatile double v[TEST_SAMPLE_SIZE];
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			v[i] = 0;
		auto start = std::chrono::high_resolution_clock::now();
		{
			for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
				v[i] = I.get_random_variable();
		}
		auto stop = std::chrono::high_resolution_clock::now();
		auto est_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() / TEST_SAMPLE_SIZE;
		std::cerr << est_time << " nanoseconds to generate random value" << std::endl;
#ifndef _DEBUG
		EXPECT_GT(100,est_time) << "Has running time of "<< est_time << " nanoseconds";
#else
		EXPECT_GT(5000,est_time);
#endif // !_DEBUG
	}
	TEST(function_param, generate_quality)
	{
		constexpr int TEST_SAMPLE_SIZE = 100;
		auto I = function_param<double>(0., 1.);
		volatile double v[TEST_SAMPLE_SIZE];
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			v[i] = 0;
		auto start = std::chrono::high_resolution_clock::now();
		{
			for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
				v[i] = I.get_random_variable();
		}
		int same = 0;
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			for (auto j = i; j-- > 0;)
				if (v[i] == v[j])
					same++;
		EXPECT_EQ(same, 0) << "produced matching random numbers" << same << "times" << std::endl;
	}
	TEST(function_param, perf_compare_test)
	{
		constexpr int TEST_SAMPLE_SIZE = 100;
		auto I = function_param_tests<double>(0., 1.);
		volatile double v[TEST_SAMPLE_SIZE];
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			v[i] = 0;
		auto start = std::chrono::high_resolution_clock::now();
		{
			for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
				v[i] = I.binded_gen0();
		}
		auto stop = std::chrono::high_resolution_clock::now();
		int same = 0;
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			for (auto j = i; j-- > 0;)
				if (v[i] == v[j])
					same++;
		EXPECT_EQ(same, 0) << "produced matching random numbers" << same << "times" << std::endl;
		auto est_time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() / TEST_SAMPLE_SIZE;
		std::cerr << est_time << " nanoseconds to generate random value by bindedgen0" << std::endl;
		start = std::chrono::high_resolution_clock::now();
		{
			for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
				v[i] = I.binded_gen1();
		}
		stop = std::chrono::high_resolution_clock::now();
		same = 0;
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			for (auto j = i; j-- > 0;)
				if (v[i] == v[j])
					same++;
		EXPECT_EQ(same, 0) << "produced matching random numbers" << same << "times" << std::endl;
		auto est_time1 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() / TEST_SAMPLE_SIZE;
		std::cerr << est_time1 << " nanoseconds to generate random value by bindeden1" << std::endl;
		start = std::chrono::high_resolution_clock::now();
		{
			for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
				v[i] = I.binded_gen2();
		}
		stop = std::chrono::high_resolution_clock::now();
		same = 0;
		for (auto i = TEST_SAMPLE_SIZE; i-- > 0;)
			for (auto j = i; j-- > 0;)
				if (v[i] == v[j])
					same++;
		EXPECT_EQ(same, 0) << "produced matching random numbers" << same << "times" << std::endl;
		auto est_time2 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() / TEST_SAMPLE_SIZE;
		std::cerr << est_time1 << " nanoseconds to generate random value by bibndedgen2" << std::endl;
#ifndef _DEBUG
		ASSERT_TRUE(est_time >= est_time1) << "Simple_rand has running time of " << est_time1 << " nanoseconds" <<
			std::endl << "True_rand   has running time of " << est_time << " nanoseconds";
#else
		EXPECT_GT(5000, est_time);
#endif // !_DEBUG
	}
}