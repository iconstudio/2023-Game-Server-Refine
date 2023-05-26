export module Core.Asynchron;
import Net.Context;
import Core.Session;

export namespace core
{
	using net::Operation;

	template<typename T, typename ID>
	class Asynchron
		: public Session<T, ID>
		, public net::Context
	{
	public:
		using base = Session<T, ID>;
		using crtp_type = T;
		using id_type = ID;

		explicit constexpr Asynchron(const ID& id) noexcept
			: base(id), Context()
		{}

		explicit constexpr Asynchron(ID&& id) noexcept
			: base(static_cast<ID&&>(id)), Context()
		{}

		constexpr ~Asynchron() noexcept
		{}

		Asynchron(const Asynchron& other) noexcept = delete;
		Asynchron& operator=(const Asynchron& other) noexcept = delete;
		Asynchron(Asynchron&& other) noexcept = default;
		Asynchron& operator=(Asynchron&& other) noexcept = default;
	};
}
