#pragma once

#include <vector>
#include <functional>
#include "TypeTraitsHelper.h"

namespace Fogo {
	template <typename ... Args>
	class Subject
	{
	private:
		using Function = std::function<void(Args...)>;
		std::vector<Function> functions;
	public:
		
		struct Registerer {
		private:
			Subject * subject;
		public:
			void operator()(const Function & function)
			{
				subject->registerSubscriber(function);
			}
			Registerer(Subject * subject) : subject(subject) {}
		};

		void publish(Args ... args) const
		{
			for (const auto & function : functions) function(args...);
		}
		
		void registerSubscriber(const Function & function)
		{
			functions.emplace_back(function);
		}
		
		Registerer getRegisterer()
		{
			return Registerer(this);
		}
	};
}