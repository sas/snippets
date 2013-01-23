#ifndef TYPE_LISTS_HH_
# define TYPE_LISTS_HH_

/* Needed for SortedInsert. */
# include <boost/mpl/if.hpp>

/* Type definitions. */

template<typename... Types>
struct TypeList;

template<typename Head, typename... Tail>
struct TypeList<Head, Tail...>
{
  typedef Head              type;
  typedef TypeList<Tail...> next;
};

template<typename Head>
struct TypeList<Head>
{
  typedef Head              type;
};

/* Size operation. */

template<typename TL>
struct TypeList_Size;

template<>
struct TypeList_Size<TypeList<>>
{
  static const int value = 0;
};

template<typename Head, typename... Tail>
struct TypeList_Size<TypeList<Head, Tail...>>
{
  static const int value = 1 + TypeList_Size<TypeList<Tail...>>::value;
};

/* IndexOf operation. */

template<typename TL, typename T>
struct TypeList_IndexOf;

template<typename T>
struct TypeList_IndexOf<TypeList<>, T>
{
  static const int value = -1;
};

template<typename T, typename... Tail>
struct TypeList_IndexOf<TypeList<T, Tail...>, T>
{
  static const int value = 0;
};

template<typename Head, typename... Tail, typename T>
struct TypeList_IndexOf<TypeList<Head, Tail...>, T>
{
private:
  static const int tail_value = TypeList_IndexOf<TypeList<Tail...>, T>::value;
public:
  static const int value = tail_value == -1 ? -1 : 1 + tail_value;
};

/* At operation. */

template<typename TL, int Pos>
struct TypeList_At;

template<typename Head, typename... Tail>
struct TypeList_At<TypeList<Head, Tail...>, 0>
{
  typedef Head value;
};

template<typename Head, typename... Tail, int Pos>
struct TypeList_At<TypeList<Head, Tail...>, Pos>
{
  typedef typename TypeList_At<TypeList<Tail...>, Pos - 1>::value value;
};

/* Merge operation. */

template<typename TL1, typename TL2>
struct TypeList_Merge;

template<typename... L1, typename... L2>
struct TypeList_Merge<TypeList<L1...>, TypeList<L2...>>
{
  typedef TypeList<L1..., L2...> value;
};

/* Sub operation. */

template<typename TL, typename T>
struct TypeList_Sub;

template<typename T, typename... Tail>
struct TypeList_Sub<TypeList<T, Tail...>, T>
{
  typedef TypeList<T, Tail...> value;
};

template<typename Head, typename... Tail, typename T>
struct TypeList_Sub<TypeList<Head, Tail...>, T>
{
  typedef typename TypeList_Sub<TypeList<Tail...>, T>::value value;
};

/* SortedInsert operation. */

template<typename T, typename TL, template<typename, typename> class Compare>
struct TypeList_SortedInsert;

template<typename T, template<typename, typename> class Compare>
struct TypeList_SortedInsert<T, TypeList<>, Compare>
{
  typedef TypeList<T> value;
};

template<typename T, typename Head, typename... Tail, template<typename, typename> class Compare>
struct TypeList_SortedInsert<T, TypeList<Head, Tail...>, Compare>
{
  typedef typename boost::mpl::if_c<
    Compare<T, Head>::value,
    typename TypeList_Merge<TypeList<Head>, typename TypeList_SortedInsert<T, TypeList<Tail...>, Compare>::value>::value,
    TypeList<T, Head, Tail...>
  >::type value;
};

/* Sort operation. */

template<typename TL, template<typename, typename> class Compare>
struct TypeList_Sort;

template<template<typename, typename> class Compare>
struct TypeList_Sort<TypeList<>, Compare>
{
  typedef TypeList<> value;
};

template<typename Head, typename... Tail, template<typename, typename> class Compare>
struct TypeList_Sort<TypeList<Head, Tail...>, Compare>
{
  typedef typename TypeList_SortedInsert<
    Head,
    typename TypeList_Sort<TypeList<Tail...>, Compare>::value,
    Compare
  >::value value;
};

#endif /* !TYPE_LISTS_HH_ */
