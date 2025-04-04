#pragma once
#include <array>

constexpr signed int MAX_COMPONENTS = 8;

struct Entity {
	explicit Entity(unsigned long long idP) : id{ idP } {}
	unsigned long long id;
	std::array<signed int, MAX_COMPONENTS> components{ -1, -1, -1, -1, -1, -1, -1, -1 };
};

 