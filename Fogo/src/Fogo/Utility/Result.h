#pragma once

#include <variant>
#include <optional>

namespace Fogo::Utility {

	template <typename E, typename V = void>
	class Result final {
	/*
	
	¬Œ÷‚ÌŒ‹‰Ê‚Æ¸”s‚ÌƒGƒ‰[‚ğ•Ô‚·D
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
	‚İ‚½‚¢‚ÈŠ´‚¶‚É‚©‚¯‚é

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

	template <typename E>
	class Result<E, void> final{
	private:
		std::optional<E> error;
		bool success;

	public:
		Result(const E & e) : error(e), success(false) {}
		Result() : success(true) {
			std::cout << "pokeee" << std::endl;
		}

		bool operator==(const E & e) const {
			if (success) return false;
			return e == error.value();
		}

		operator bool() const {
			return success;
		}
	};
}
