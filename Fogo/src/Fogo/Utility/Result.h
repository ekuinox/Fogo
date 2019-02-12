#pragma once

#include <variant>
#include <optional>

namespace Fogo::Utility {

	template <typename E, typename V>
	class Result final {
	private:
		std::variant<E, V> variant;
		bool success;

	public:
		Result(const E & e) : variant(e), success(false) {}
		Result(const V & v) : variant(v), success(true) {}
		Result(const Result &) = delete;
		Result & operator=(const Result &) = delete;

		bool operator==(const E & e) const {
			if (success) return false;
			return e == std::get<E>(variant);
		}

		operator bool() const {
			return success;
		}

		std::optional<V> get() const {
			if (success) return std::move(std::get<V>(variant));
			return std::nullopt;
		}

		const V & operator->() const {
			return std::get<V>(variant);
		}

		const V & operator*() const {
			return std::get<V>(variant);
		}
	};

}
