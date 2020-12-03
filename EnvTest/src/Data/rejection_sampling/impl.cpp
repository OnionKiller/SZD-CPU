#include "../modell_factory.h"
#include "impl.h"

void rejection_sampler::setData(simple_failure_times ftimes)
{
	this->failures_ = ftimes;
}
