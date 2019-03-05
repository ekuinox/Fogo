#pragma once

namespace Fogo {
	class Degree {
	private:
		float raw;
	public:
		Degree(float initValue);
		Degree();
		float toRadian() const;
		operator float() const;
		Degree & operator=(const float newValue);
	};
}
