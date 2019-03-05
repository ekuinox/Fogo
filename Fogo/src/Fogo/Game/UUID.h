#pragma once

#include <random>
#include <unordered_map>
#include <string>
#include <iostream>

namespace Fogo {

	struct UUID {
		using Engine = std::independent_bits_engine<std::mt19937, 64, std::uint64_t>;

		static auto Generate() {
			static const auto & seed = std::random_device()();
			static Engine engine(seed);

			return engine();
		}

		const Engine::result_type raw;

		UUID() : raw(Generate()) {}

		struct Hash {
			using result_type = std::size_t;

			std::size_t operator()(const UUID & key) const {
				return std::hash<Engine::result_type>{}(key.raw);
			}
		};

		bool operator==(const UUID & rhs) const {
			return rhs.raw == raw;
		}

		bool operator!=(const UUID & rhs) const {
			return rhs.raw != raw;
		}

		UUID & operator=(const UUID & v) {
			const_cast<Engine::result_type&>(raw) = v.raw;
			return * this;
		}

		std::string toString() const {
			return std::to_string(raw);
		}

		friend std::ostream & operator<<(std::ostream& stream, const UUID & uuid) {
			return stream << uuid.raw;
		}
	};

}
