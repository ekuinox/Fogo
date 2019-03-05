#pragma once

#include "../Component.h"

namespace Fogo {
	struct Meta : public Component {
		std::string name;
		std::string tag;
		int layer;
		Meta(std::string name_, std::string tag_, int layer_);
	};
}
