#include "./Degree.h"
#include <DirectXMath.h>

using Fogo::Degree;

Degree::Degree(float initValue) : raw(initValue) {

}

Degree::Degree() : Degree(0) {

}

float Degree::toRadian() const {
	return DirectX::XMConvertToRadians(raw);
}

Degree::operator float() const {
	return raw;
}

Degree & Degree::operator=(const float newValue) {
	raw = newValue;
	return *this;
}
