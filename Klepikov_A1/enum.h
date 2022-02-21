#ifndef ENUM_H
#define ENUM_H

enum class query_compare
{
	none, // не определен
	l,		// меньше
	g,		// больше
	le,		// меньше или равен
	ge,		// больше или равен
	eq,		// равен
	ne,		// не равен
};

enum class parse_results
{
	none,
	SUCCESS,
	MEMORY,
	COMPARE,
	REPLACE,
};

enum class query_type
{
	none = -1, // не определен
	find,			 // найти
	replace,	 // заменить
};
enum class query_operation
{
	none, // не определен
	land, // все слова
	lor,	// хотя бы одно
};

enum errors
{
	SUCCESS = 0,
	OPEN = -10,	 // не открылся файл
	EMPTY = -20, // пустой файл
	MEM = -30,	 // не выделилась память
	ARGS = -40,	 // неправильные аргументы
	EMPTY_S = -50,
};
#endif // ENUM_H