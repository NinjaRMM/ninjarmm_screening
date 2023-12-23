
#ifndef JOB_H_
#define JOB_H_

#include <iostream>


namespace ninjans{

	class Job
	{
	public:
		Job(){}
		virtual ~Job()	{
	//		std::cout << "Job DTOR " << std::endl;
		}
		virtual std::string Title() const;
		virtual std::string Work() const =0; // Returns a string about what it does for work.
		virtual bool IsRemoteWorker() const;
		virtual bool HasUniform() const;

	protected:
		std::string m_title;
		bool m_isRemoteWorker;
		bool m_hasUniform;

	};
}

#endif /* JOB_H_ */
