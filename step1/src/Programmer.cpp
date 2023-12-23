
#include "Programmer.h"

namespace ninjans
{
	Programmer::Programmer()
	{
		m_hasUniform = false;
		m_isRemoteWorker = true;
		m_title = "Software Engineer";
	}

	Programmer::~Programmer()
	{
//		std::cout << "Programmer DTOR" <<std::endl;
	}

	std::string Programmer::Work() const
	{
		return "I write computer programs.";
	}

	bool Programmer::WriteCode() const
	{
		return true;
	}

}
