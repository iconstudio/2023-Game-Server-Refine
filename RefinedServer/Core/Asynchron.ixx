export module Core.Asynchron;
import Utility;
import Net.Context;
import Core.Session;

export namespace core
{
	template<typename T, session_identifiers ID>
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
			: base(util::move(id)), Context()
		{}

		constexpr ~Asynchron() noexcept
		{}

		Asynchron(const Asynchron& other) noexcept = delete;
		Asynchron& operator=(const Asynchron& other) noexcept = delete;
		Asynchron(Asynchron&& other) noexcept = default;
		Asynchron& operator=(Asynchron&& other) noexcept = default;
	};
}
