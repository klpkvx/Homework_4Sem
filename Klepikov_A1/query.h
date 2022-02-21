#ifndef QUERY_H
#define QUERY_H
#include "enum.h"
#include "stdio.h"
#include "cstring"
#include <memory>
#define LEN 1234
#define SPACE 256
using namespace std;
int get_word(char *curr_pos, char *s_words, char *spaces, char **saveptr);
class Query
{
private:
	query_type type = query_type::none;
	query_operation operation = query_operation::none;
	query_compare compare = query_compare::none;
	int s_count = 0;
	char *s_words[LEN];			// s
	char *spaces = nullptr; // t
	char *x_words[LEN];			// for task3
	int x_count = 0;				// for task3

public:
	Query()
	{
		for (int i = 0; i < LEN; i++)
		{
			s_words[i] = nullptr;
			x_words[i] = nullptr;
		}
	}
	query_compare define_compare(const char *x) // < > = <= >= <>
	{
		if (x[0] != '\0')
		{
			if (x[1] == '\0')
			{
				if (x[0] == '<')
				{
					return query_compare::l;
				}
				if (x[0] == '>')
				{
					return query_compare::g;
				}
				if (x[0] == '=')
				{
					return query_compare::eq;
				}
				return query_compare::none;
			}
			else
			{
				if (x[0] == '<' && x[1] == '=')
				{
					return query_compare::le;
				}
				if (x[0] == '>' && x[1] == '=')
				{
					return query_compare::ge;
				}
				if (x[0] == '<' && x[1] == '>')
				{
					return query_compare::ne;
				}
				return query_compare::none;
			}
		}
		else
			return query_compare::none;
	}

	parse_results parse(int task, char *s, char *t, char *x = nullptr)
	{
		set_task(task, x);
		spaces = t;
		set_s_words(s);

		if (task == 3)
		{
			set_x_words(x);
		}
		if (((task == 4) && (compare == query_compare::none)) || ((task == 5) && (compare == query_compare::none)))
			return parse_results::COMPARE;

		return parse_results::SUCCESS;
	}

	int apply(FILE *istream_, FILE *ostream_)
	{
		if ((type == query_type::find) && (operation == query_operation::lor) && (compare == query_compare::eq))
		{
			return task1(istream_, ostream_);
		}
		if ((type == query_type::find) && (operation == query_operation::land) && (compare == query_compare::eq))
		{
			return task2(istream_, ostream_);
		}

		if ((type == query_type::replace) && (operation == query_operation::land) && (compare == query_compare::eq))
		{
			return task3(istream_, ostream_);
		}

		if ((type == query_type::find) && (operation == query_operation::lor))
		{
			return task4(istream_, ostream_);
		}

		if ((type == query_type::find) && (operation == query_operation::land))
		{
			return task5(istream_, ostream_);
		}

		return 0; // надо возвращать ответ
	}

	int get_s_count() const { return s_count; }
	int get_x_count() const { return x_count; }
	void print_s_words(char *s) const
	{
		printf("STRING:%s\nWORDS COUNT:%d\n", s, s_count);

		for (int i = 0; i < s_count; i++)
			printf("\tWORD[%d]=%s\n", i, s_words[i]);
		printf("\n");
		;
	}

	void print_x_words(char *x) const
	{
		printf("STRING:%s\nWORDS COUNT:%d\n", x, x_count);

		for (int i = 0; i < x_count; i++)
			printf("\tWORD[%d]=%s\n", i, x_words[i]);
		printf("\n");
		;
	}

private:
	void set_s_words(char *s)
	{
		char *saveptr = nullptr;
		char *res = strtok_r(s, spaces, &saveptr);
		if (res)
			s_words[s_count++] = res;

		while (res)
		{
			res = strtok_r(nullptr, spaces, &saveptr);
			if (res)
				s_words[s_count++] = res;
		}
	}

	void set_x_words(char *x)
	{
		char *saveptr = nullptr;
		char *res = strtok_r(x, spaces, &saveptr);
		if (res)
			x_words[x_count++] = res;

		while (res)
		{
			res = strtok_r(nullptr, spaces, &saveptr);
			if (res)
				x_words[x_count++] = res;
		}
	}
	void set_task(int task, char *x)
	{
		switch (task)
		{
		case 1: // найти хотя бы одно слово из s в хотя бы одном слове строки из бд
			type = query_type::find;
			operation = query_operation::lor;
			compare = query_compare::eq;
			break;
		case 2: // найти хотя бы одно слово из s во всех словах строки из бд
			type = query_type::find;
			operation = query_operation::land;
			compare = query_compare::eq;
			break;
		case 3: // заменить все вхождения слова из строки s на слово из x и вывести все строки из бд
			type = query_type::replace;
			operation = query_operation::land;
			compare = query_compare::eq;
			break;
		case 4: // найти хотя бы одно слово из s, где хотя бы одно слово в строке из бд удовлетворяет условию x
			type = query_type::find;
			operation = query_operation::lor;
			compare = define_compare(x);
			break;
		case 5: // найти хотя бы одно слово из s, где каждое слово в строке из бд удовлетворяет условию x
			type = query_type::find;
			operation = query_operation::land;
			compare = define_compare(x);
			break;
		default:
			type = query_type::none;
			operation = query_operation::land;
			compare = query_compare::none;
		}
	}

	int task1(FILE *istream_, FILE *ostream_)
	{
		char buf[LEN];
		char *saveptr = nullptr;
		int counter = 0;
		int flag = 0;
		if (s_count == 0)
			return 0;

		while (fgets(buf, LEN, istream_))
		{
			char curr_str[LEN];
			int i = 0;
			for (; buf[i]; i++)
			{
				curr_str[i] = buf[i];
				if (curr_str[i] == '\n')
					curr_str[i] = '\0';
			}
			curr_str[i] = '\0';
			i = 0;

			char *word = nullptr;
			word = strtok_r(curr_str, spaces, &saveptr);
			while (word)
			{
				int i = 0;
				flag = 0;
				while (s_words[i])
				{
					if (s_words[i])
					{
						if (strcmp(word, s_words[i++]) == 0)
						{
							flag = 1;
							break;
						}
					}
				}
				if (flag == 1)
				{
					flag = 0;
					counter++;
					fprintf(ostream_, "%s", buf);
					break;
				}

				word = strtok_r(nullptr, spaces, &saveptr);
			}
		}
		return counter;
	}

	int task2(FILE *istream_, FILE *ostream_)
	{
		char buf[LEN];
		char *saveptr = nullptr;
		int counter = 0;
		int flag = -1;
		if (s_count == 0)
			return 0;

		while (fgets(buf, LEN, istream_))
		{
			char curr_str[LEN];
			int i = 0;
			for (; buf[i]; i++)
			{
				curr_str[i] = buf[i];
				if (curr_str[i] == '\n')
					curr_str[i] = '\0';
			}
			curr_str[i] = '\0';

			char *word = nullptr;
			i = 0;
			flag = -1;

			word = strtok_r(curr_str, spaces, &saveptr);
			while (word)
			{
				flag = -1;
				for (int i = 0; s_words[i] && flag; i++)
				{
					if (strcmp(word, s_words[i]) != 0)
						flag = 1;
					else
						flag = 0;
				}
				if (flag)
					break;
				word = strtok_r(nullptr, spaces, &saveptr);
			}
			if (flag == 0)
			{
				counter++;
				fprintf(ostream_, "%s", buf);
			}
		}
		return counter;
	}

	int task3(FILE *istream_, FILE *ostream_)
	{

		char buf[LEN];
		int counter = 0;
		if (x_count == 0 || s_count == 0)
		{
			while (fgets(buf, LEN, istream_))
			{
				fprintf(ostream_, "%s", buf);
			}
			return 0;
		}

		while (fgets(buf, LEN, istream_))
		{
			char curr_str[LEN];
			int i = 0;
			for (; buf[i]; i++)
			{
				curr_str[i] = buf[i];
				if (curr_str[i] == '\n')
					curr_str[i] = '\0';
			}
			curr_str[i] = '\0';

			char *curr_pos = curr_str;
			char *saveptr = nullptr;

			int flag3 = 0;
			int j = 0;
			int flag = 1;
			int start = 0;
			int index = 0;
			while (*curr_pos)
			{
				int flag2 = 0;
				flag = 1;
				for (i = 0; s_words[i] && flag; i++)
				{
					if (start == 0)
						flag2 = 1;
					else
					{
						for (j = 0; spaces[j]; j++)
							if (spaces[j] == curr_pos[-1])
								flag2 = 1;
					}
					if (get_word(curr_pos, s_words[i], spaces, &saveptr) && flag2)
					{
						index = i;
						flag = 0;
					}
				}
				if (flag == 0)
				{
					if (x_words[index])
					{
						flag3 = 1;
						fprintf(ostream_, "%s", x_words[index]);
					}
					curr_pos = saveptr;
				}
				else
				{
					fprintf(ostream_, "%c", *curr_pos);
					curr_pos++;
				}
				start = 1;
			}
			if (flag3)
				counter++;
			if (!feof(istream_))
				fprintf(ostream_, "\n");
		}
		return counter;
	}
	int task4(FILE *istream_, FILE *ostream_)
	{
		char buf[LEN];
		char *saveptr = nullptr;
		int counter = 0;
		int flag = 0;
		if (s_count == 0)
			return 0;
		while (fgets(buf, LEN, istream_))
		{
			char curr_str[LEN];
			int i = 0;
			for (; buf[i]; i++)
			{
				curr_str[i] = buf[i];
				if (curr_str[i] == '\n')
					curr_str[i] = '\0';
			}
			curr_str[i] = '\0';
			i = 0;

			char *word = nullptr;
			word = strtok_r(curr_str, spaces, &saveptr);
			while (word)
			{
				int i = 0;
				flag = 0;
				while (s_words[i])
				{
					if (s_words[i])
					{
						if (check_compare(word, s_words[i++]))
						{
							flag = 1;
							break;
						}
					}
				}
				if (flag == 1)
				{
					flag = 0;
					counter++;
					fprintf(ostream_, "%s", buf);
					break;
				}

				word = strtok_r(nullptr, spaces, &saveptr);
			}
		}
		return counter;
	}

	int task5(FILE *istream_, FILE *ostream_)
	{
		char buf[LEN];
		char *saveptr = nullptr;
		int counter = 0;
		int flag = -1;
		if (s_count == 0)
			return 0;

		while (fgets(buf, LEN, istream_))
		{
			char curr_str[LEN];
			int i = 0;
			for (; buf[i]; i++)
			{
				curr_str[i] = buf[i];
				if (curr_str[i] == '\n')
					curr_str[i] = '\0';
			}
			curr_str[i] = '\0';

			char *word = nullptr;
			i = 0;
			flag = -1;

			word = strtok_r(curr_str, spaces, &saveptr);
			while (word)
			{
				flag = -1;
				for (int i = 0; s_words[i] && flag; i++)
				{
					if ((!check_compare(word, s_words[i])) != 0)
						flag = 1;
					else
						flag = 0;
				}
				if (flag)
					break;
				word = strtok_r(nullptr, spaces, &saveptr);
			}
			if (flag == 0)
			{
				counter++;
				fprintf(ostream_, "%s", buf);
			}
		}
		return counter;
	}
	// >0 если первая строка больше второй
	int check_compare(char *word, char *s_words_)
	{
		int res = strcmp(word, s_words_);
		switch (compare)
		{
		case query_compare::l:
			return (res < 0 ? 1 : 0);
		case query_compare::g:
			return (res > 0 ? 1 : 0);
		case query_compare::eq:
			return (res = 0 ? 1 : 0);
		case query_compare::le:
			return (res <= 0 ? 1 : 0);
		case query_compare::ge:
			return (res >= 0 ? 1 : 0);
		case query_compare::ne:
			return (res == 0 ? 0 : 1);
		default:
			return 0;
			break;
		}
	}
};

int get_word(char *curr_pos, char *s_words, char *spaces, char **saveptr)
{
	int i = 0;
	while (s_words[i] && curr_pos[i] == s_words[i])
		i++;
	if (!s_words[i])
	{
		for (int j = 0; spaces[j]; j++)
		{
			if (curr_pos[i] == spaces[j] || !curr_pos[i])
			{
				*saveptr = curr_pos + i;
				return 1;
			}
		}
	}
	return 0;
}

#endif // QUERY_H