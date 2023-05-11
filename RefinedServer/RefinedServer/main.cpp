#include "pch.hpp"
#include "Framework.hpp"

Framework* myFramework;

import Utility.Union;
int main()
{
	myFramework = new Framework{};

	myFramework->Awake();
	myFramework->Start();
	myFramework->Update();
}
namespace util::test
{
	void test_union2() noexcept
	{
		using aa_t = Union<int, void, unsigned long, float>;

		constexpr aa_t aa{};
		using aa_0_t = aa_t::element_type<0>;
		static_assert(util::is_same_v<aa_0_t, int>, "int");
		using aa_1_t = aa_t::element_type<1>;
		//static_assert(is_same_v<aa_1_t, int>, "unsigned long");
		using aa_2_t = aa_t::element_type<2>;
		//static_assert(is_same_v<aa_2_t, int>, "float");
		//using aa_3_t = aa_t::element_type<3>;

		constexpr aa_0_t aa_ty_0_v = 0;
		//constexpr aa_1_t aa_ty_1_v;
		constexpr aa_2_t aa_ty_2_v = 0;

		aa.myPlace;
		aa.mySize;
		aa.relativeIndex<0>;
		aa.relativeIndex<1>;
		aa.relativeIndex<2>;
		aa.relativeIndex<3>;

		constexpr bool a_has_0 = aa.has_value<0>();
		constexpr bool a_has_1 = aa.has_value<1>();
		constexpr bool a_has_2 = aa.has_value<2>();

		using bb_t = Union<int, unsigned long, float, double>;
		bb_t bb0{};
		//bb_t bb1{};
		bb_t bb1(in_place_type<float>, integral_constant<size_t, 0>{}, 4000.034124f);
		bb0.set(0);

		bb0 = bb1;
		bb0.reset();

		// PlacedVariant<integral_constant<size_t, 0>, int, unsigned long, float, double>
		auto& rb0_tset_0 = bb0.try_set<0>(0);
		// PlacedVariant<integral_constant<size_t, 1>, unsigned long, float, double>
		auto& rb0_tset_1 = bb0.try_set<1>(0UL);

		constexpr Union<int, int, int> cc{};
		constexpr Union<bool, int, long> dd{};
		constexpr Union<float, unsigned long long, char> ee{};
		constexpr Union<double, unsigned char, short> ff{};
	}
}
