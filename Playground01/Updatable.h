#pragma once

#include <type_traits>

class Updatable {
public:
	virtual void update() = 0;

	template <typename T>
	static constexpr bool IsDerived() {
		return std::is_base_of<Updatable, T>();
	}

	template <typename T>
	static constexpr bool IsSame() {
		return std::is_same<Updatable, T>();
	}
};
