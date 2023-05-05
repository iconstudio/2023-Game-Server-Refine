export module Net.Session.Asynchron;
import Utility;
import Net.Context;
import Net.Session;

export namespace net
{
	template<typename T, session_identifiers ID>
	class Asynchron
		: public ISession<T, ID>
		, public Context
	{
	public:
		using base = ISession<T, ID>;
		using crtp_type = T;
		using id_type = ID;

		Asynchron(Asynchron&& other) noexcept = default;
		Asynchron& operator=(Asynchron&& other) noexcept = default;

		explicit constexpr Asynchron(const ID& id) noexcept
			: base(id), Context()
		{}

		explicit constexpr Asynchron(ID&& id) noexcept
			: base(util::move(id)), Context()
		{}

		constexpr ~Asynchron() noexcept = default;
	};
}
