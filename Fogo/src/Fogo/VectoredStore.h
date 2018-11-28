#pragma once
#include <vector>
#include <stdexcept>

class VectoredStore
{
public:
	class cannnot_get_element : public std::exception
	{
	public:
		cannnot_get_element()
			: std::exception("cannnot get element error !") {}
	};

private:
	template<typename T>
	class Container
	{
	private:
		std::vector<T> __container;
		Container() : __container({}) {}
	public:

		void add(T elm)
		{
			__container.emplace_back(elm);
		}

		T & get()
		{
			if (__container.size() < 1) throw cannnot_get_element();
			return __container.back();
		}

		static Container & GetInstance()
		{
			static const auto instance = new Container();
			return * instance;
		}
	};

public:
	template <typename T, typename ... Pairs>
	static void Reserve()
	{
		Container<T>::GetInstance();
		if constexpr (sizeof...(Pairs) > 0) Reserve<Pairs...>();
	}

	template <typename T>
	static void Add(T elm)
	{
		Container<T>::GetInstance().add(elm);
	}

	template <typename T>
	static T & Get()
	{
		return Container<T>::GetInstance().get();
	}
};

