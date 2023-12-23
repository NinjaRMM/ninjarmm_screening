#include "Job.h"

namespace ninjans{


	std::string ninjans::Job::Title() const
	{
		return m_title;
	}

	bool ninjans::Job::IsRemoteWorker() const
	{
		return m_isRemoteWorker;
	}

	bool ninjans::Job::HasUniform() const
	{
		return m_hasUniform;
	}

}
