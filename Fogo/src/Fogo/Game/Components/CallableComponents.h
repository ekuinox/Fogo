#pragma once

#include "./CallableComponentBase.h"

namespace Fogo {
	using Updater = CallableComponentBase<0>;
	using Renderer = CallableComponentBase<1>;
	using Initializer = CallableComponentBase<2>;
	using Finalizer = CallableComponentBase<3>;
	using Starter = CallableComponentBase<4>;
	using Stopper = CallableComponentBase<5>;
}