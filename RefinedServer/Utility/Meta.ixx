module;
#include <type_traits>
export module Utility.Meta;

namespace meta::detail
{
	template <typename Ty, size_t>
	using enumerator = Ty;

	template <typename, typename, template <typename...> typename>
	struct enumerate;

	template <typename T, size_t... Indices, template <typename...> class Predicate>
	struct enumerate<T, std::index_sequence<Indices...>, Predicate>
	{
		using type = typename Predicate<enumerator<T, Indices>...>;
	};

	// construct a sequence consisting of repetitions of T
	template <size_t Count, typename T, template <typename...> typename Predicate>
	using create = typename enumerate<T, std::make_index_sequence<Count>, Predicate>::template type;

	// Extract the Index-th type from Sequence
	template <typename Seq, size_t Index, typename = void>
	struct __at;

#ifdef __clang__
	template <template <typename...> class Seq, typename... _Types, size_t Index>
	struct at<Seq<_Types...>, Index, void_t<__type_pack_element<Index, _Types...>>>
	{
		using type = __type_pack_element<Index, _Types...>;
	};
#else // ^^^ __clang__ / !__clang__ vvv
	template <typename... _VoidPtrs>
	struct enumerate_for
	{
		template <typename T, typename... Rests>
		static consteval T _Eval(_VoidPtrs..., T*, Rests*...) noexcept; // undefined
	};

	template <typename T>
	consteval std::type_identity_t<T>* AsPointer() noexcept
	{
		return nullptr;
	}

	template <template <typename...> typename Seq, size_t Index, typename... Ts>
		requires (Index < sizeof...(Ts))
	struct __at<Seq<Ts...>, Index>
	{
		using type = typename decltype(create<Index, void*, enumerate_for>::_Eval(AsPointer<Ts>()...));
	};
#endif // __clang__
}

export namespace meta
{
	/// <summary>
	/// a sequence of types
	/// </summary>
	template <typename... Ts>
	struct MetaList {};
	struct MetaNil {};

	template <typename Void, template <typename...> typename Fn, typename... Args>
	struct wrapper;

	template <template <typename...> typename Fn, typename... Args>
	struct wrapper<std::void_t<Fn<Args...>>, Fn, Args...>
	{
		using type = typename Fn<Args...>;
	};

	// invoke meta-callable Functor with Arguments
	template <typename Fn, typename... Args>
	using invoke = typename Fn::template result<Args...>;

	template <typename Fn, typename... Args>
	using invoke_r = typename Fn::template result<Args...>::type;

	// encapsulate a template into a meta-callable type
	template <template <typename...> typename Fn>
	struct wrap
	{
		template <typename... Args>
		using result = typename wrapper<void, Fn, Args...>::template type;
	};

	// construct a meta-callable that passes its arguments and Args to Functor
	template <typename Fn, typename... Args>
	struct bind
	{
		template <typename... Ty>
		using result = typename Fn::template result<Ty..., Args...>;
	};

	// unpack List into the parameters of meta-callable Functor
	template <typename Fn, typename Seq>
	struct apply;

	template <class Fn, class Seq>
	using apply_t = typename apply<Fn, Seq>::type;

	// invoke meta-callable Functor with the parameters of a template specialization
	template <typename Fn, template <typename...> typename Seq, typename... Ts>
	struct apply<Fn, Seq<Ts...>>
	{
		using type = typename Fn::template result<Seq<Ts...>>::type;
	};

	// invoke meta-callable Functor with the elements of an integer_sequence
	template <typename Fn, typename T, T... Values>
	struct apply<Fn, std::integer_sequence<T, Values...>>
	{
		using type = typename Fn::template result<std::integral_constant<T, Values>...>;
	};

	// transform sequence of Ty... into sequence of Functor<Ty...>
	template <typename Fn, class Seq>
	struct transform;

	template <template <typename...> typename Seq, typename Fn, typename... Ts>
	struct transform<Fn, Seq<Ts...>>
	{
		using type = Seq<invoke_r<Fn, Ts>...>;
	};

	template <typename Fn, class Seq>
	using transform_t = typename transform<Fn, Seq>::type;

	template <typename Seq>
	struct front;

	template <typename Seq>
	using front_t = typename front<Seq>::template type;

	template <template <typename...> typename Seq, typename First, typename... Rests>
	struct front<Seq<First, Rests...>>
	{
		using type = First;
	};

	template <template <typename...> typename Seq>
	struct front<Seq<>>
	{};

	template <typename Seq>
	struct back;

	template <typename Seq>
	using back_t = typename back<Seq>::template type;

	template <template <typename...> typename Seq, typename Fty, typename Nty, typename... Rests>
	struct back<Seq<Fty, Nty, Rests...>> : public back<Seq<Nty, Rests...>>
	{};

	template <template <typename...> typename Seq, typename Last>
	struct back<Seq<Last>>
	{
		using type = Last;
	};

	template <template <typename...> typename Seq>
	struct back<Seq<>>
	{};

	template <typename LTy, typename RTy>
	struct push;

	template <typename LTy, typename RTy>
	using push_t = typename push<LTy, RTy>::template type;

	template <template <typename...> typename Seq, typename T, typename... Fronts>
	struct push<Seq<Fronts...>, T>
	{
		using type = Seq<T, Fronts...>;
	};

	template <template <typename...> typename Seq, typename T, typename... Fronts>
	struct push<T, Seq<Fronts...>>
	{
		using type = Seq<T, Fronts...>;
	};

	template <typename LTy, typename RTy>
	struct push_back;

	template <typename LTy, typename RTy>
	using push_back_t = typename push_back<LTy, RTy>::template type;

	template <template <typename...> typename Seq, typename T, typename... Backs>
	struct push_back<Seq<Backs...>, T>
	{
		using type = Seq<Backs..., T>;
	};

	template <template <typename...> typename Seq, typename T, typename... Backs>
	struct push_back<T, Seq<Backs...>>
	{
		using type = Seq<T, Backs...>;
	};

	template <typename Seq>
	struct pop;

	template <typename Seq>
	using pop_t = typename pop<Seq>::template type;

	template <template <typename...> typename Seq, typename First, typename... Rests>
	struct pop<Seq<First, Rests...>>
	{
		using type = Seq<Rests...>;
	};

	template <template <typename...> typename Seq>
	struct pop<Seq<>>
	{};

	template <typename Seq>
	struct pop_back;

	template <typename Seq>
	using pop_back_t = typename pop_back<Seq>::template type;

	template <template <typename...> typename Seq, typename First, typename Next, typename... Rests>
	struct pop_back<Seq<First, Next, Rests...>> : public push_back<First, pop_back_t<Seq<Next, Rests...>>>
	{};

	template <template <typename...> typename Seq, typename First, typename Last>
	struct pop_back<Seq<First, Last>>
	{
		using type = Seq<First>;
	};

	template <template <typename...> typename Seq, typename Last>
	struct pop_back<Seq<Last>>
	{};

	template <template <typename...> typename Seq>
	struct pop_back<Seq<>>
	{};

	// find a type in sequence
	template <typename T, typename...>
	struct find;

	template <typename T, typename... Ts>
	using find_t = typename find<T, Ts...>::type;

	template <typename T, typename... Rests>
	struct find<T, T, Rests...>
	{
		using type = T;
	};

	template <typename T, typename First, typename... Rests>
	struct find<T, First, Rests...> : public find<T, Rests...>
	{};

	template <typename T>
	struct find<T>
	{};

	// determine if a type is in sequence
	template <typename T, typename...>
	struct included;

	template <typename T, typename... Ts>
	constexpr bool included_v = included<T, Ts...>::value;

	template <typename T, typename... Rests>
	struct included<T, T, Rests...> : public std::true_type
	{};

	template <typename T, typename First, typename... Rests>
	struct included<T, First, Rests...> : public included<T, Rests...>
	{};

	template <typename T>
	struct included<T> : public std::false_type
	{};

	// determine if a type is in specialized template
	template <typename T, typename R>
	struct included_range;

	template <typename T, typename R>
	constexpr bool included_range_v = included_range<T, R>::value;

	template <typename T, template<typename...> typename R, typename... Ts>
	struct included_range<T, R<T, Ts...>>
		: public std::true_type
	{};

	template <typename T, template<typename...> typename R, typename First, typename... Rests>
	struct included_range<T, R<First, Rests...>>
		: public included_range<T, R<Rests...>>
	{};

	template <typename T, template<typename...> typename R>
	struct included_range<T, R<>>
		: public std::false_type
	{};

	// merge several lists into one
	template <typename...>
	struct concat;

	template <typename... Lists>
	using concat_t = typename concat<Lists...>::type;

	template <template <typename...> typename List>
	struct concat<List<>>
	{
		using type = List<>;
	};

	template <template <typename...> typename List, typename... Ts>
	struct concat<List<Ts...>>
	{
		using type = List<Ts...>;
	};

	template <template <typename...> typename List, typename... Ts0, typename... Ts1>
	struct concat<List<Ts0...>, List<Ts1...>>
	{
		using type = List<Ts0..., Ts1...>;
	};

	template <template <typename...> typename List, typename... Ts0, typename... Ts1, typename... Ts2>
	struct concat<List<Ts0...>, List<Ts1...>, List<Ts2...>>
	{
		using type = List<Ts0..., Ts1..., Ts2...>;
	};

	template <template <typename...> typename List, typename... Ts0, typename... Ts1, typename... Ts2, typename... Rests>
	struct concat<List<Ts0...>, List<Ts1...>, List<Ts2...>, Rests...>
	{
		using type = concat<List<Ts0..., Ts1..., Ts2...>, Rests...>::type;
	};

	// construct a sequence consisting of repetitions of T
	template <size_t Count, typename T, template <typename...> typename Predicate>
	using create = typename detail::enumerate<T, std::make_index_sequence<Count>, Predicate>::template type;

	// get the type at Index in Sequence
	template <typename Seq, size_t Index>
	using at = typename detail::__at<Seq, Index>::template type;

	// transform a list of lists of elements into a single list containing those elements
	template <typename ListOfLists>
	using unzip = apply<wrap<concat>, ListOfLists>;

	template <typename ListOfLists>
	using unzip_t = typename unzip<ListOfLists>::type;

	template <typename>
	struct _Meta_cartesian_product_;

	// find the n-ary Cartesian Product of the lists in the input list
	template <typename ListOfLists>
	using cartesian_product = typename _Meta_cartesian_product_<ListOfLists>::type;

	template <template <typename...> typename Seq>
	struct _Meta_cartesian_product_<Seq<>>
	{
		using type = Seq<>;
	};

	template <template <typename...> typename Seq0, template <typename...> typename Seq1
		, typename... Ts>
	struct _Meta_cartesian_product_<Seq0<Seq1<Ts...>>>
	{
		using type = Seq0<Seq1<Ts>...>;
	};

	template <template <typename...> class Seq0, template <typename...> class Seq1
		, typename... Ts
		, typename... Rests>
	struct _Meta_cartesian_product_<Seq0<Seq1<Ts...>, Rests...>>
	{
		using Adder = wrap<push_back>;
		using Predicate = bind<Adder, Ts...>;
		using Next = cartesian_product<Seq0<Rests...>>;
		using Merged = Seq0<transform_t<bind<Adder, Ts>, Next>...>;

		using type = unzip_t<Merged>;
	};
}

namespace meta
{
	static inline void test_metafunctions() noexcept
	{
		using test_seq = MetaList<int, float, double>;
		constexpr front_t<test_seq> a000{};
		constexpr back_t<test_seq> a001{};

		constexpr pop_back<test_seq> a002{};
		constexpr pop_back_t<test_seq> t_a002{};

		constexpr push_t<test_seq, long> a010{};
		constexpr front_t<push_t<test_seq, long>> a011{};

		constexpr push_t<long, test_seq> a012{};
		constexpr pop_t<push_t<long, test_seq>> a013{};

		constexpr wrap<front> a014{};
		constexpr wrap<pop> a015{};
		constexpr wrap<pop_back> a016{};
		constexpr wrap<push> a017{};

		constexpr wrap<front>::result<test_seq> _a014{};
		constexpr wrap<pop>::result<test_seq> _a015{};

		constexpr wrap<pop_back>::result<test_seq> _a016{};
		constexpr wrap<pop_back>::result<test_seq>::type t_a016{};

		constexpr wrap<push>::result<test_seq, long> _a017{};
		constexpr wrap<push>::result<test_seq, long>::type t_a017{};
		constexpr wrap<pop>::result<test_seq> a018{};
		constexpr wrap<pop>::result<test_seq>::type t_a018{};
		constexpr wrap<pop>::result<push_t<long, test_seq>> a019{};
		constexpr wrap<pop>::result<push_t<long, test_seq>>::type t_a019{};

		constexpr apply<wrap<front>, test_seq> a020{};
		constexpr apply_t<wrap<front>, test_seq> a021{};

		constexpr bind<wrap<back>> _a030{};
		constexpr bind<wrap<back>>::result<test_seq> t_a030{};
		constexpr bind<wrap<back>>::result<test_seq>::type tt_a030{};

		constexpr bind<wrap<push>, test_seq> a031{};
		constexpr bind<wrap<push>, test_seq>::result<long> t_a031{};
		constexpr bind<wrap<push>, test_seq>::result<long>::type tt_a031{};

		constexpr bind<wrap<pop_back>, test_seq> a032{};
		constexpr bind<wrap<pop_back>, test_seq>::result<> t_a032{};
		constexpr bind<wrap<pop_back>, test_seq>::result<>::type tt_a032{};

		constexpr apply_t<wrap<front>, test_seq> a041{};
		constexpr apply_t<wrap<back>, test_seq> a042{};
		constexpr apply_t<wrap<pop>, test_seq> a043{};
		constexpr apply_t<wrap<pop_back>, test_seq> a044{};

		using SeqOfSeq = MetaList<test_seq, test_seq, test_seq>;
		constexpr apply_t<wrap<back>, SeqOfSeq> a045{};
		constexpr front_t<SeqOfSeq> a046{};
		constexpr back_t<SeqOfSeq> a047{};

		constexpr pop_back<SeqOfSeq> a048{};
		constexpr pop_back_t<SeqOfSeq> t_a048_0{};
		constexpr pop_t<pop_back_t<SeqOfSeq>> t_a048_1{};

		constexpr back_t<push_t<SeqOfSeq, long>> a049{};

		using NewSeq = transform_t<bind<wrap<push>, test_seq>, test_seq>;

		constexpr NewSeq a050{};
		constexpr pop_back<NewSeq> a051{};
		constexpr pop_back_t<NewSeq> t_a051{};

		constexpr front<NewSeq> a052{};
		constexpr front_t<NewSeq> t_a052{};

		constexpr back_t<front_t<NewSeq>> a053_0{};
		constexpr front_t<back_t<NewSeq>> a054_1{};

		constexpr pop_back<transform_t<bind<wrap<push>, test_seq>, test_seq>> a055{};
		constexpr pop_back_t<transform_t<bind<wrap<push>, test_seq>, test_seq>> t_a055{};
		constexpr back_t<pop_back_t<transform_t<bind<wrap<push>, test_seq>, test_seq>>> a056{};
		constexpr front_t<pop_back_t<transform_t<bind<wrap<push>, test_seq>, test_seq>>> a057{};
		constexpr back_t<pop_back_t<transform_t<bind<wrap<push>, test_seq>, test_seq>>> a058{};
		constexpr back_t<back_t<pop_back_t<transform_t<bind<wrap<push>, test_seq>, test_seq>>>> a059{};

		constexpr create<5, int, MetaList> a060{};

		constexpr detail::__at<test_seq, 0> a070{};
		constexpr at<test_seq, 0> r_a070{};
		constexpr detail::__at<test_seq, 1> a071{};
		constexpr at<test_seq, 1> r_a071{};
		constexpr detail::__at<test_seq, 2>::type a072{};
		constexpr at<test_seq, 2> r_a072{};
		//constexpr __at<test_seq, 3> a073{};
		//constexpr at<test_seq, 3> r_a073{};

		constexpr unzip<test_seq> a074{};
		constexpr unzip_t<test_seq> t_a074{};
		constexpr unzip<SeqOfSeq> a075{};
		constexpr unzip_t<SeqOfSeq> t_a075{};
		constexpr unzip<transform_t<bind<wrap<push>, test_seq>, test_seq>> a076{};
		constexpr unzip_t<transform_t<bind<wrap<push>, test_seq>, test_seq>> t_a076{};

		constexpr cartesian_product<NewSeq> a080{};
		using Product = cartesian_product<NewSeq>;

		constexpr at<Product, 0> r_a080_at0{};
		constexpr at<at<Product, 0>, 0> r_a080_at0_at0{};

		constexpr at<at<at<Product, 0>, 0>, 0>        r_a080_at0_at0_at0{};
		constexpr at<at<at<at<Product, 0>, 0>, 0>, 0> r_a080_at0_at0_at0_at0{};
		constexpr at<at<at<at<Product, 0>, 0>, 0>, 1> r_a080_at0_at0_at0_at1{};
		constexpr at<at<at<Product, 0>, 0>, 1>        r_a080_at0_at0_at1{};
		constexpr at<at<at<at<Product, 0>, 0>, 1>, 0> r_a080_at0_at0_at1_at0{};
		constexpr at<at<at<at<Product, 0>, 0>, 1>, 1> r_a080_at0_at0_at1_at1{};

		constexpr at<at<Product, 0>, 1> r_a080_at0_at1{};
		constexpr at<at<Product, 0>, 2> r_a080_at0_at2{};
		constexpr at<at<Product, 0>, 3> r_a080_at0_at3{};

		constexpr at<Product, 1> r_a080_at1{};
		constexpr at<at<Product, 1>, 0> r_a080_at1_at0{};
		constexpr at<at<Product, 1>, 1> r_a080_at1_at1{};
		constexpr at<at<Product, 1>, 2> r_a080_at1_at2{};
		constexpr at<at<Product, 1>, 3> r_a080_at1_at3{};

		constexpr at<Product, 2> r_a080_at2{};
		constexpr at<at<Product, 2>, 0> r_a080_at2_at0{};
		constexpr at<at<Product, 2>, 1> r_a080_at2_at1{};
		constexpr at<at<Product, 2>, 2> r_a080_at2_at2{};
		constexpr at<at<Product, 2>, 3> r_a080_at2_at3{};

		constexpr at<Product, 3> r_a080_at3{};
		constexpr at<at<Product, 3>, 0> r_a080_at3_at0{};
		constexpr at<at<Product, 3>, 1> r_a080_at3_at1{};
		constexpr at<at<Product, 3>, 2> r_a080_at3_at2{};
		constexpr at<at<Product, 3>, 3> r_a080_at3_at3{};

		//constexpr at<Product, 4> r_a080_at4{};
		//constexpr at<Product, 5> r_a080_at5{};
		//constexpr at<Product, 6> r_a080_at6{};
		//constexpr at<Product, 7> r_a080_at7{};
		//constexpr at<Product, 8> r_a080_at8{};
		//constexpr at<Product, 9> r_a080_at9{};

		constexpr front_t<at<Product, 0>> r_a080_at0_f{};
		constexpr front_t<front_t<at<Product, 0>>> r_a080_at0_ff{};
		constexpr at<front_t<front_t<at<Product, 0>>>, 0> r_a080_at0_ff_at0{};
		constexpr at<front_t<front_t<at<Product, 0>>>, 1> r_a080_at0_ff_at1{};

		constexpr back_t<front_t<at<Product, 0>>> r_a080_at0_bf{};
		//constexpr at<back_t<front_t<at<Product, 0>>>, 0> r_a080_at0_bf_at0{};
		//constexpr at<back_t<front_t<at<Product, 0>>>, 1> r_a080_at0_bf_at1{};

		constexpr front_t<front_t<at<Product, 1>>> r_a080_at1_ff{};
		constexpr at<front_t<front_t<at<Product, 1>>>, 0> r_a080_at1_ff_at0{};
		constexpr at<front_t<front_t<at<Product, 1>>>, 1> r_a080_at1_ff_at1{};

		constexpr front_t<front_t<at<Product, 2>>> r_a080_at2_ff{};
		constexpr at<front_t<front_t<at<Product, 2>>>, 0> r_a080_at2_ff_at0{};
		constexpr at<front_t<front_t<at<Product, 2>>>, 1> r_a080_at2_ff_at1{};

		constexpr front_t<front_t<front_t<at<Product, 0>>>> r_a080_at0_fff{};

		constexpr front_t<cartesian_product<NewSeq>> a080_f{};
		constexpr front_t<front_t<cartesian_product<NewSeq>>> a080_ff{};
		constexpr front_t<front_t<front_t<cartesian_product<NewSeq>>>> a080_fff{};
	}
}
