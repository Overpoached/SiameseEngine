#pragma once

template<typename T>
struct Component
{
	T current{};
	T next{};

	void Swap() noexcept;
};