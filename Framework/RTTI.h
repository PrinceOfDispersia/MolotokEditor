/* A simple C++ reflection mark-up library. The strength 
of this library is that it is entirely static (all the 
reflection information is built without using dynamic 
memory allocation), and it is exremely compact, allowing
you to declare the members of a structure right where 
that struct is declared, leading to minimal risk of 
version mismatch.

This code is placed in the public domain by Jon Watte.
http://www.enchantedage.com/cpp-reflection
Version 2009-04-20
*/
#ifndef RTTI_H
#define RTTI_H


struct TypeBase
{
	virtual void Marshal(void *, void const *) const = 0;
	virtual void Demarshal(void const *, void *) const = 0;
	virtual char const *name() const = 0;
	virtual size_t size() const = 0;
};

template<typename T> struct Type : TypeBase
{
	static Type<T> instance;
	// custom marshaling is handled by template specialization
	void Marshal(void *dst, void const *src) const { memcpy(dst, src, sizeof(T)); }
	void Demarshal(void const *src, void *dst) const { memcpy(dst, src, sizeof(T)); }
	char const *name() const { return typeid(T).name(); }
	size_t size() const { return sizeof(T); }
};

template <typename T> T& instance() {
	static T t;
	return t;
}

template<typename T, typename Q>
TypeBase *get_type(Q T::*mem) {
	return &instance<Type<Q> >();
}

struct member_t
{
	char const *name;
	TypeBase *type;
	size_t offset;
};

struct ReflectionBase
{
	inline void ReflectionConstruct()
	{
		//  todo: register myself in some table, etc
		members();
		memberCount();
		name();
		size();
		all_members.clear();
	}

	
	virtual size_t size() const = 0;
	virtual char const *name() const = 0;
	virtual size_t memberCount() const = 0;
	virtual member_t const *members() const = 0;

	std::vector<member_t> all_members;
};


#define MEMBER(x) \
{ #x, get_type(&T::x), (size_t)&((T*)0)->x },

#define CREATE_METADATA(_type, _mems) \
template<typename T> struct _info : ReflectionBase { \
	/* overrides used by ReflectionBase */ \
	inline size_t size() const { return sizeof(T); } \
	inline char const *name() const { return #_type; } \
	inline size_t memberCount() const { size_t cnt; get_members(cnt); return cnt; } \
	inline member_t const *members() const { size_t cnt; return get_members(cnt); } \
	_info() \
	{ \
		size_t cnt;\
		ReflectionConstruct();\
		for(size_t i = 0 ; i < memberCount() ; i++) \
		{ \
			all_members.push_back(get_members(cnt)[i]); \
		} \
	} \
	static inline member_t const *get_members(size_t &cnt) { \
	static member_t members[] = { _mems }; \
	cnt = sizeof(members) / sizeof(members[0]); \
	return members; \
} \
	static inline _info<T> &info() { \
	return instance<_info<T> >(); \
} \
}; \
std::vector<member_t> & members() { return _type::_theInfo.all_members; } \
static _info<_type> _theInfo; \
static _info<_type> & info() { return _theInfo; }\

#define ADD_METADATA(_class,_mems)\
template<typename T>\
struct meta\
{\
	meta()\
	{\
		static member_t members[] = { _mems }; \
		size_t cnt = sizeof(members) / sizeof(members[0]); \
		for(size_t i = 0 ; i < cnt ; i++)\
		{\
			info().all_members.push_back(members[i]);\
		}\
	}\
};\
meta<_class> extraMetadata;\

#endif