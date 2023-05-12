module;
#include <system_error>
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <memory>
#include <array>
#include <algorithm>
#include <ranges>
export module Utility.Array;

export namespace util
{
	using std::initializer_list;

	template<typename Elem, size_t Length>
	using array_iterator = std::_Array_iterator<Elem, Length>;

	template<typename Elem, size_t Length>
	using array_const_iterator = std::_Array_const_iterator<Elem, Length>;

	struct empty_array_elemement { constexpr explicit empty_array_elemement() = default; };

	template<typename Elem, size_t Length>
	class Array
	{
	public:
		using value_type = Elem;
		using const_value_type = const Elem;
		using pointer = Elem*;
		using const_pointer = const Elem*;
		using reference = Elem&;
		using const_reference = const Elem&;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using iterator = array_iterator<Elem, Length>;
		using const_iterator = array_const_iterator<Elem, Length>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		static inline constexpr bool NothrowInitializable = std::is_nothrow_default_constructible_v<Elem>;
		static inline constexpr bool NothrowDestructible = std::is_nothrow_destructible_v<Elem>;

		constexpr Array() noexcept(NothrowInitializable) = default;
		constexpr ~Array() noexcept(NothrowDestructible) = default;

		constexpr Array(const Elem(&raw_data)[Length]) noexcept(std::is_nothrow_copy_assignable_v<Elem>)
			requires std::copyable<Elem>
		{
			if (std::is_constant_evaluated())
			{
				std::copy(std::cbegin(raw_data), std::cend(raw_data), begin());
			}
			else
			{
				std::memcpy(myData, raw_data, Length * sizeof(Elem));
			}
		}

		template<size_t OLength>
		constexpr Array(const Elem(&raw_data)[OLength]) noexcept(std::is_nothrow_copy_assignable_v<Elem>)
			requires (std::copyable<Elem>&& OLength <= Length)
		{
			if (std::is_constant_evaluated())
			{
				std::copy(std::cbegin(raw_data), std::cend(raw_data), begin());
			}
			else
			{
				std::memcpy(myData, raw_data, Length * sizeof(Elem));
			}
		}

		template<std::convertible_to<Elem> Elem2>
		explicit constexpr Array(const std::initializer_list<Elem2> elements) noexcept(std::is_nothrow_copy_assignable_v<Elem>&& std::is_nothrow_copy_assignable_v<Elem2>)
		{
			std::copy(elements.begin(), elements.end(), begin());
		}

		constexpr Array(const Array& other) noexcept(std::is_nothrow_copy_assignable_v<Elem>)
			requires std::copyable<Elem>: Array()
		{
			if (std::is_constant_evaluated())
			{
				std::copy(other.cbegin(), other.cend(), begin());
			}
			else
			{
				std::memcpy(myData, other.data(), Length);
			}
		}

		constexpr Array(Array&& other) noexcept(std::is_nothrow_move_assignable_v<Elem>)
			requires std::movable<Elem> : Array()
		{
			if (std::is_constant_evaluated())
			{
				std::move(other.cbegin(), other.cend(), begin());
			}
			else
			{
				std::memcpy(myData, other.data(), Length);
			}
		}

		inline constexpr Array& operator=(const Array& other)
			noexcept(std::is_nothrow_move_assignable_v<Elem>)
			requires std::movable<Elem>
		{
			if (std::is_constant_evaluated())
			{
				std::copy(other.cbegin(), other.cend(), begin());
			}
			else
			{
				std::memcpy(myData, other.data(), Length);
			}

			return *this;
		}

		inline constexpr Array& operator=(Array&& other)
			noexcept(std::is_nothrow_move_assignable_v<Elem>)
			requires std::movable<Elem>
		{
			if (std::is_constant_evaluated())
			{
				std::move(other.cbegin(), other.cend(), begin());
			}
			else
			{
				std::memcpy(myData, other.data(), Length);
			}

			return *this;
		}

		template<std::invocable<value_type&> Fn>
		inline constexpr void apply(Fn&& function) noexcept
		{
			for (value_type& elem : *this)
			{
				std::forward<Fn>(function)(elem);
			}
		}

		template<std::invocable<const_value_type&> Fn>
		inline constexpr void apply(Fn&& function) const noexcept
		{
			for (const_value_type& elem : *this)
			{
				std::forward<Fn>(function)(elem);
			}
		}

		template<std::convertible_to<value_type> OElem, size_t OLength>
			requires (0 < OLength)
		inline constexpr void assign(const Array<OElem, OLength>& other) noexcept(std::is_nothrow_assignable_v<value_type, OElem>)
		{
			auto src = other.cbegin();

			for (iterator it = begin(); it != end() && src != other.cend(); (void) ++it)
			{
				*it = *(src++);
			}
		}

		template<std::convertible_to<value_type> OElem, size_t OLength>
			requires (0 < OLength)
		inline constexpr void assign(Array<OElem, OLength>&& other) noexcept(std::is_nothrow_assignable_v<value_type, OElem>)
		{
			auto src = other.cbegin();

			for (iterator it = begin(); it != end() && src != other.cend(); (void) ++it)
			{
				*it = std::move(*(src++));
			}
		}

		inline constexpr void fill(const value_type& fill_value) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
		{
			std::fill_n(myData, Length, fill_value);
		}

		inline constexpr void fill(value_type&& fill_value) noexcept(std::is_nothrow_move_assignable_v<value_type>)
		{
			const auto& value = std::move(fill_value);
			std::fill_n(myData, Length, value);
		}

		inline constexpr void swap(std::array<Elem, Length>& other_data) noexcept(std::is_nothrow_swappable_v<std::array<Elem, Length>>)
		{
			auto oit = other_data.cbegin();

			for (iterator it = begin(); it != end(); ++it, (void) ++oit)
			{
				std::iter_swap(it, oit);
			}
		}

		inline constexpr void swap(Array& other) noexcept(std::is_nothrow_swappable_v<Elem>)
		{
			auto oit = other.begin();

			for (iterator it = begin(); it != end(); ++it, (void) ++oit)
			{
				std::iter_swap(it, oit);
			}
		}

		[[nodiscard]]
		inline constexpr iterator begin() noexcept
		{
			return iterator{ myData, 0 };
		}

		[[nodiscard]]
		inline constexpr iterator end() noexcept
		{
			return iterator{ myData, Length };
		}

		[[nodiscard]]
		inline constexpr const_iterator begin() const noexcept
		{
			return const_iterator{ myData, 0 };
		}

		[[nodiscard]]
		inline constexpr const_iterator end() const noexcept
		{
			return const_iterator{ myData, Length };
		}

		[[nodiscard]]
		inline constexpr reverse_iterator rbegin() noexcept
		{
			return reverse_iterator{ end() };
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator{ end() };
		}

		[[nodiscard]]
		inline constexpr reverse_iterator rend() noexcept
		{
			return reverse_iterator{ begin() };
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator{ begin() };
		}

		[[nodiscard]]
		inline constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}

		[[nodiscard]]
		inline constexpr const_iterator cend() const noexcept
		{
			return end();
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator crend() const noexcept
		{
			return rend();
		}

		[[nodiscard]]
		inline constexpr
			reference
			at(const size_type& index)&
		{
			if (Length <= index)
			{
				throw std::length_error{ "배열의 크기 참조 오류" };
			}

			return myData[index];
		}

		[[nodiscard]]
		inline constexpr
			const_reference
			at(const size_type& index) const&
		{
			if (Length <= index)
			{
				throw std::length_error{ "배열의 크기 참조 오류" };
			}

			return myData[index];
		}

		[[nodiscard]]
		inline constexpr
			value_type&&
			at(const size_type& index)&&
		{
			if (Length <= index)
			{
				throw std::length_error{ "배열의 크기 참조 오류" };
			}

			return std::move(myData[index]);
		}

		[[nodiscard]]
		inline constexpr
			const_value_type&&
			at(const size_type& index) const&&
		{
			if (Length <= index)
			{
				throw std::length_error{ "배열의 크기 참조 오류" };
			}

			return std::move(myData[index]);
		}

		[[nodiscard]]
		inline constexpr reference
			operator[](_In_range_(0, Length - 1) const size_type& index) & noexcept
		{
			return myData[index];
		}

		[[nodiscard]]
		inline constexpr
			const_reference
			operator[](_In_range_(0, Length - 1) const size_type& index) const& noexcept
		{
			return myData[index];
		}

		[[nodiscard]]
		inline constexpr
			value_type&&
			operator[](_In_range_(0, Length - 1) const size_type& index) && noexcept
		{
			return std::move(myData[index]);
		}

		[[nodiscard]]
		inline constexpr
			const_value_type&&
			operator[](_In_range_(0, Length - 1) const size_type& index) const&& noexcept
		{
			return std::move(myData[index]);
		}

		[[nodiscard]]
		inline constexpr reference front() & noexcept
		{
			return myData[0];
		}

		[[nodiscard]]
		inline constexpr const_reference front() const& noexcept
		{
			return myData[0];
		}

		[[nodiscard]]
		inline constexpr value_type&& front() && noexcept
		{
			return std::move(myData[0]);
		}

		[[nodiscard]]
		inline constexpr const value_type&& front() const&& noexcept
		{
			return std::move(myData[0]);
		}

		[[nodiscard]]
		inline constexpr reference back() & noexcept
		{
			return myData[Length - 1];
		}

		[[nodiscard]]
		inline constexpr const_reference back() const& noexcept
		{
			return myData[Length - 1];
		}

		[[nodiscard]]
		inline constexpr value_type&& back() && noexcept
		{
			return std::move(myData[Length - 1]);
		}

		[[nodiscard]]
		inline constexpr const value_type&& back() const&& noexcept
		{
			return std::move(myData[Length - 1]);
		}

		[[nodiscard]]
		inline constexpr pointer data() & noexcept
		{
			return myData;
		}

		[[nodiscard]]
		inline constexpr const_pointer data() const& noexcept
		{
			return myData;
		}

		[[nodiscard]]
		inline constexpr pointer&& data() && noexcept
		{
			return std::move(myData);
		}

		[[nodiscard]]
		inline constexpr size_type size() const noexcept
		{
			return Length;
		}

		[[nodiscard]]
		static inline consteval size_type max_size() noexcept
		{
			return Length;
		}

		[[nodiscard]]
		static inline consteval bool empty() noexcept
		{
			return false;
		}

	private:
		Elem myData[Length];
	};

	template<typename Elem>
	class Array<Elem, 0>
	{
	public:
		using value_type = Elem;
		using const_value_type = const Elem;
		using pointer = Elem*;
		using const_pointer = const Elem*;
		using reference = Elem&;
		using const_reference = const Elem&;

		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using iterator = array_iterator<Elem, 0>;
		using const_iterator = array_const_iterator<Elem, 0>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		constexpr Array() noexcept = default;
		constexpr ~Array() noexcept = default;

		template<size_t OLength>
		constexpr Array([[maybe_unused]] const Elem(&raw_data)[OLength]) noexcept
		{}

		template<std::convertible_to<Elem> Elem2>
		explicit constexpr Array([[maybe_unused]] const std::initializer_list<Elem2> elements) noexcept
		{}

		constexpr Array(const Array& other) noexcept = default;
		constexpr Array(Array&& other) noexcept = default;
		inline constexpr Array& operator=(const Array& other) noexcept = default;
		inline constexpr Array& operator=(Array&& other) noexcept = default;

		template<std::invocable<value_type&> Fn>
		inline constexpr void apply([[maybe_unused]] Fn&& function) noexcept
		{}

		template<std::invocable<const_value_type&> Fn>
		inline constexpr void apply([[maybe_unused]] Fn&& function) const noexcept
		{}

		template<std::convertible_to<value_type> OElem, size_t OLength>
		inline constexpr void assign([[maybe_unused]] const Array<OElem, OLength>& other) noexcept
		{}

		template<std::convertible_to<value_type> OElem, size_t OLength>
		inline constexpr void assign([[maybe_unused]] Array<OElem, OLength>&& other) noexcept
		{}

		inline constexpr void fill([[maybe_unused]] const value_type& fill_value) noexcept
		{}

		inline constexpr void fill([[maybe_unused]] value_type&& fill_value) noexcept
		{}

		inline constexpr void swap([[maybe_unused]] Array& other) noexcept
		{}

		[[nodiscard]]
		inline constexpr iterator begin() noexcept
		{
			return iterator{ nullptr, 0 };
		}

		[[nodiscard]]
		inline constexpr iterator end() noexcept
		{
			return iterator{ nullptr, 1 };
		}

		[[nodiscard]]
		inline constexpr const_iterator begin() const noexcept
		{
			return const_iterator{ nullptr, 0 };
		}

		[[nodiscard]]
		inline constexpr const_iterator end() const noexcept
		{
			return const_iterator{ nullptr, 1 };
		}

		[[nodiscard]]
		inline constexpr reverse_iterator rbegin() noexcept
		{
			return reverse_iterator{ end() };
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator{ end() };
		}

		[[nodiscard]]
		inline constexpr reverse_iterator rend() noexcept
		{
			return reverse_iterator{ begin() };
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator{ begin() };
		}

		[[nodiscard]]
		inline constexpr const_iterator cbegin() const noexcept
		{
			return begin();
		}

		[[nodiscard]]
		inline constexpr const_iterator cend() const noexcept
		{
			return end();
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}

		[[nodiscard]]
		inline constexpr const_reverse_iterator crend() const noexcept
		{
			return rend();
		}

		[[nodiscard]]
		inline constexpr
			reference
			at(const size_type& index)&
		{
			return *begin();
		}

		[[nodiscard]]
		inline constexpr
			const_reference
			at(const size_type& index) const&
		{
			return *cbegin();
		}

		[[nodiscard]]
		inline constexpr
			value_type&&
			at(const size_type& index)&&
		{
			return std::move(*begin());
		}

		[[nodiscard]]
		inline constexpr
			const_value_type&&
			at(const size_type& index) const&&
		{
			return std::move(*cbegin());
		}

		[[nodiscard]]
		inline constexpr reference
			operator[](const size_type& index) & noexcept
		{
			return *begin();
		}

		[[nodiscard]]
		inline constexpr
			const_reference
			operator[](const size_type& index) const& noexcept
		{
			return *cbegin();
		}

		[[nodiscard]]
		inline constexpr
			value_type&&
			operator[](const size_type& index) && noexcept
		{
			return std::move(*begin());
		}

		[[nodiscard]]
		inline constexpr
			const_value_type&&
			operator[](const size_type& index) const&& noexcept
		{
			return std::move(*begin());
		}

		[[nodiscard]]
		inline constexpr reference front() & noexcept
		{
			return *begin();
		}

		[[nodiscard]]
		inline constexpr const_reference front() const& noexcept
		{
			return *cbegin();
		}

		[[nodiscard]]
		inline constexpr value_type&& front() && noexcept
		{
			return std::move(*begin());
		}

		[[nodiscard]]
		inline constexpr const value_type&& front() const&& noexcept
		{
			return std::move(*cbegin());
		}

		[[nodiscard]]
		inline constexpr reference back() & noexcept
		{
			return *begin();
		}

		[[nodiscard]]
		inline constexpr const_reference back() const& noexcept
		{
			return *begin();
		}

		[[nodiscard]]
		inline constexpr value_type&& back() && noexcept
		{
			return std::move(*begin());
		}

		[[nodiscard]]
		inline constexpr const value_type&& back() const&& noexcept
		{
			return std::move(*cbegin());
		}

		[[nodiscard]]
		inline constexpr pointer data() & noexcept
		{
			return nullptr;
		}

		[[nodiscard]]
		inline constexpr const_pointer data() const& noexcept
		{
			return nullptr;
		}

		[[nodiscard]]
		inline constexpr pointer&& data() && noexcept
		{
			return nullptr;
		}

		[[nodiscard]]
		inline constexpr size_type size() const noexcept
		{
			return 0;
		}

		[[nodiscard]]
		static inline consteval size_type max_size() noexcept
		{
			return 0;
		}

		[[nodiscard]]
		static inline consteval bool empty() noexcept
		{
			return true;
		}

	private:
		empty_array_elemement myData[1];
	};

	using ::std::copy;
	using ::std::transform;
	using ::std::swap;
}

export template<typename Elem, size_t Length>
inline constexpr bool std::ranges::enable_borrowed_range<util::Array<Elem, Length>> = true;

export namespace std
{
	template<typename Elem, size_t Length>
	inline constexpr void swap(::util::Array<Elem, Length>& lhs, ::util::Array<Elem, Length>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}
