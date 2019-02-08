#pragma once

class Renderable {
public:
	virtual void render() const = 0;

	template <typename T>
	static constexpr bool IsDerived() {
		return std::is_base_of<Renderable, T>();
	}

	template <typename T>
	static constexpr bool IsSame() {
		return std::is_same<Renderable, T>();
	}
};
