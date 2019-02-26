#pragma once

#include <variant>
#include <optional>

namespace Fogo::Utility {

	template <typename E, typename V>
	class Result final {
	/*
	
	成功時の結果と失敗時のエラーを返す．
	Result<ErrorType, const char *> foo(int n) {
		if (expression) {
			return "OK Value";
		}
		return ErrorType::NotOK;
	}
	void main() {
		const auto & result = foo(10);
		if (result) {
			ok(*result);
		}
		else if (result == ErrorType::NotOK) {
			fail();
		}
	}
	みたいな感じにかける

	*/
	private:
		std::variant<E, V> variant;
		bool success;

	public:
		Result(const E & e) : variant(e), success(false) {}
		Result(const V & v) : variant(v), success(true) {}

		bool operator==(const E & e) const {
			if (success) return false;
			return e == std::get<E>(variant);
		}

		operator bool() const {
			return success;
		}

		std::optional<V> get() const {
			if (success) return std::get<V>(variant);
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
