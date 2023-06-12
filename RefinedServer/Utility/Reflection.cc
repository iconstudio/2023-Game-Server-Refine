import <string_view>;
import Utility.Reflection;

#define MAKE_PACK(...) \
	::reflex::tag_types<__VA_ARGS__>

#define MAKE_REFLECTION(Type, ID, Name, Parents, Children) \
	template<> \
	struct ::reflex::reflect_traits<Type> \
	{\
		using type = Type;\
		using parents = Parents;\
		using children = Children;\
		\
		static constexpr size_t type_id = ID;\
		static constexpr std::string_view name = Name;\
		static constexpr size_t size = sizeof(Type);\
	}

#define MAKE_VOID_REFLECTION(Type, ID, Name, Parents, Children) \
	template<> \
	struct ::reflex::reflect_traits<Type> \
	{\
		using type = Type;\
		using parents = Parents;\
		using children = Children;\
		\
		static constexpr size_t type_id = ID;\
		static constexpr std::string_view name = Name;\
		static constexpr size_t size = 0;\
	}

#define MAKE_ARRAY_REFLECTION(Type, Name) \
	template<size_t Length> \
	struct ::reflex::reflect_traits<Type[Length]> \
	{\
		using type = Type;\
		using parents = ::reflex::reflect_traits<Type>::parents;\
		using children = ::reflex::reflect_traits<Type>::children;\
		\
		static constexpr size_t type_id = ::reflex::reflect_traits<Type>::type_id;\
		static constexpr std::string_view name = Name;\
		static constexpr size_t size = sizeof(Type) * Length;\
	}

#define MAKE_REFLECTION_NO_CHILDREN(type, id, name, ps) \
	MAKE_REFLECTION(type, id, name, ps, void)

#define MAKE_REFLECTION_NO_PARENTS(type, id, name, chs) \
	MAKE_REFLECTION(type, id, name, void, chs)

#define MAKE_REFLECTION_NO_HIERACHY(type, id, name) \
	MAKE_REFLECTION(type, id, name, void, void)
