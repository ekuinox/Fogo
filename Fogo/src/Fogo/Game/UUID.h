#pragma once

#include <random>
#include <unordered_map>
#include <string>
#include <iostream>

namespace Fogo {

	struct UUID {
		using Engine = std::independent_bits_engine<std::mt19937, 64, std::uint64_t>;

		static Engine::result_type Generate();

		const Engine::result_type raw;

		UUID();

		struct Hash {
			using result_type = std::size_t;
			std::size_t operator()(const UUID & key) const;
		};

		bool operator==(const UUID & rhs) const;
		bool operator!=(const UUID & rhs) const;
		UUID & operator=(const UUID & v);
		std::string toString() const;

		friend std::ostream & operator<<(std::ostream& stream, const UUID & uuid) {
			return stream << uuid.raw;
		}
	};
}
