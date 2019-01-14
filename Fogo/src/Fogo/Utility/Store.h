#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <functional>

namespace Fogo::Utility {

	template <typename T>
	class Container : public std::unordered_map<std::string, std::unordered_map<std::string, T>> {
	private:
		Container() : std::unordered_map<std::string, std::unordered_map<std::string, T>>({}) {}
	public:
		static Container & GetInstance() {
			static Container instance;
			return instance;
		}
	};

	class Store {
	public:
		template <class T> using ExecuteFunction = std::function<void(T)>;

	private:
		std::string storeId;

	public:
		Store(std::string storeId) : storeId(std::move(storeId)) {
			
		}

		const std::string & getStoreId() const {
			return storeId;
		}

		template <typename T>
		void insert(const std::string & id, const T & element) const {
			Container<T>::GetInstance()[storeId][id] = element;
		}

		template <typename T>
		T & get(const std::string & id) const {
			return Container<T>::GetInstance()[storeId][id];
		}

		template <class T>
		void execute(const ExecuteFunction<T> & function) const {
			for (const auto & element : Container<T>::GetInstance()[storeId]) {
				function(element.second);
			}
		}
	};
}
