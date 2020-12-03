#pragma once

/** Data interface to hold any kind of failure data, specifically:
* Incomplete data
* Data with tags like Preventive maitenance/Reapir action
*/

#include <unordered_set>
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <numeric>
#include <execution>
#include "accepted_tags.h"

class tagged_failure {
public:
	/*tag management*/
	//check if failure has tag
	virtual bool has_tag(tags tag) {
		//Cpp2020 feature 
		//return tag_list_.contains(tag);
		return tag_list_.find(tag) != tag_list_.end();
	}
	//add tag to failure
	virtual void add_tag(tags tag){
		auto insertion_result_ = tag_list_.insert(tag);
#ifdef NOTIFICATIONS
		if (!insertion_result_.second)
			std::cerr << "Notice: tried to add tag already existing (" << tagmanager::get_tag_name(tag) <<")" << std::endl;
#endif // NOTIFICATIONS
	}
	//add ultiple tags at once to failure
	virtual void add_tag(std::vector<tags> tag_list){
		for (auto&& I : tag_list)
			add_tag(I);
	}
	//remove tag from failure
	virtual void remove_tag(tags tag) {
		tag_list_.erase(tag);
	}
	//remove tags from failure
	virtual void remove_tag(std::vector<tags> tag_list) {
		for (auto&& I : tag_list)
			remove_tag(I);
	}
	//get concrete failure time to calcualte with
	virtual double get_failure_time() = 0;
	//string print data
	virtual std::string print() {
		return tags_to_stirng() + "\n";
	}
protected:
	//adding tags to the list, setting false if exist
	std::unordered_set<tags> tag_list_;	
	//tags to string
	std::string tags_to_stirng() {
		std::string return_ = "[";
		for (auto&& I : tag_list_)
		{
			return_ += tagmanager::get_tag_name(I);
			return_ += ",";
		}
		return_.replace(return_.end()-1, return_.end(), "]");
		return return_;
	}
};

class simple_failure : public tagged_failure
{
public:
	simple_failure() = delete;
	simple_failure(double failure_time) :failure_time(failure_time) {};
	virtual double get_failure_time() {
		return failure_time;
	};
	virtual std::string print() {
		return "{ x = " + std::to_string(failure_time) + " ; " + tags_to_stirng() + "}\n";
	}
protected:
	double failure_time;
};

class failure_times {
public:
	virtual void add_failure(tagged_failure failure) = delete;
	virtual std::vector<tagged_failure> get_failure_list();
protected:
	std::vector<tagged_failure> failure_list_ = {};
};

class simple_failure_times {
public:
	virtual void add_failure(simple_failure failure) { this->failure_list_.push_back(failure); };
	virtual std::vector<simple_failure> get_failure_list() { return failure_list_; };
	typedef std::vector<simple_failure>::const_iterator const_iterator;
	const_iterator begin() const {
		return failure_list_.begin();
	};
	const_iterator end() const {
		return failure_list_.end();
	};
	double const Sum(const size_t i = -1) {
		auto end = failure_list_.cbegin() + i;
		if (i < 0)
			end = failure_list_.cend();
		auto r = std::transform_reduce(std::execution::par, failure_list_.cbegin(), end, double(0.),\
			[](double a, double b)->double {return a + b; }, [](simple_failure A) {return A.get_failure_time(); });
		return r;
	};
protected:
	std::vector<simple_failure> failure_list_ = {};
};


