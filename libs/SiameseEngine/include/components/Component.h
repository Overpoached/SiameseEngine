#pragma once

namespace sengine
{
	template<typename T>
	struct Component
	{
		T current{};
		//TODO: use a double buffered ecs system for multi threading
		T next{};

		void Swap() noexcept;
	};
}