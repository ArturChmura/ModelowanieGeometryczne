#pragma once

#include "Bezier.h"

namespace MG1
{
	class InterpolatedC2 : public Bezier
	{
	public:
        explicit InterpolatedC2() = default;

        explicit InterpolatedC2(Bezier &&other);

		friend void to_json(nlohmann::ordered_json& j, const InterpolatedC2& p);
		friend void from_json(const nlohmann::ordered_json& j, InterpolatedC2& p);
	};

	class InterpolatedC0 : public Bezier
	{
	public:
		explicit InterpolatedC0() = default;

		explicit InterpolatedC0(Bezier&& other);

		friend void to_json(nlohmann::ordered_json& j, const InterpolatedC0& p);
		friend void from_json(const nlohmann::ordered_json& j, InterpolatedC0& p);
	};
}
