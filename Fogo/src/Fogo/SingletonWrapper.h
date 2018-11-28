#pragma once

namespace Fogo {

	template <class T>
	class SingletonWrapper
	{
	private:

	public:
		SingletonWrapper() = delete;
		SingletonWrapper(const SingletonWrapper &) = delete;
		SingletonWrapper& operator=(const SingletonWrapper&) = delete;

		template<typename ... Args>
		static T& GetInstance(Args ... args)
		{
			static auto instance = T(args...);
			return instance;
		}
	};

}