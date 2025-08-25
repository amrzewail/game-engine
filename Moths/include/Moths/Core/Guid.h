#pragma once

#include <random>
#include <sstream>
#include <iomanip>
#include <string>

#include <Moths/API.h>

namespace MothsEngine
{

	struct MOTHS_API Guid
	{
	private:
		std::string value;

	public:
		static Guid* NewGuid()
		{
			return new Guid();
		}


		Guid()
		{
			static thread_local std::mt19937_64 rng{ std::random_device{}() };
			static std::uniform_int_distribution<uint64_t> dist;

			uint64_t part1 = dist(rng);
			uint64_t part2 = dist(rng);

			std::ostringstream oss;
			oss << std::hex << std::setfill('0')
				<< std::setw(8) << (uint32_t)(part1 >> 32) << "-"
				<< std::setw(4) << (uint16_t)(part1 >> 16) << "-"
				<< std::setw(4) << ((uint16_t)part1 & 0x0FFF | 0x4000) << "-" // version 4
				<< std::setw(4) << ((uint16_t)(part2 >> 48) & 0x3FFF | 0x8000) << "-" // variant
				<< std::setw(12) << (part2 & 0xFFFFFFFFFFFFull);

			value = oss.str();
		}

		~Guid()
		{
		}

		std::string ToString() const
		{
			return value;
		}

		bool operator==(const Guid& other) const noexcept
		{
			return value == other.value;
		}

		bool operator!=(const Guid& other) const noexcept
		{
			return value != other.value;
		}
	};
}