#include "./UUID.h"

using namespace Fogo;

const UUID UUID::InvalidUUID = UUID();

std::size_t UUID::Hash::operator()(const UUID & key) const {
	return std::hash<Engine::result_type>{}(key.raw);
}

UUID::Engine::result_type UUID::Generate() {
	static const auto & seed = std::random_device()();
	static Engine engine(seed);

	return engine();
}

UUID::UUID() : raw(Generate()) {

}

bool UUID::operator==(const UUID & rhs) const {
	return rhs.raw == raw;
}

bool UUID::operator!=(const UUID & rhs) const {
	return rhs.raw != raw;
}

UUID & UUID::operator=(const UUID & v) {
	const_cast<Engine::result_type&>(raw) = v.raw;
	return *this;
}

std::string UUID::toString() const {
	return std::to_string(raw);
}
