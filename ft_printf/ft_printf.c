#include <unistd.h>
#include <stdarg.h>

void print_str(char *str, int *count)
{
	if (!str)
		str = "(null)";
	while (*str)
		*count += write(1, str++, 1);
}

void print_base(long long int num, int base, int *count)
{
	char *hexa = "0123456789abcdef";
	if (num < 0)
	{
		num *= -1;
		*count += write(1, "-", 1);
	}
	if (num >= base)
		print_base(num / base, base, count);
	*count += write(1, &hexa[num % base], 1);
}

int	ft_printf(const char *str, ...)
{
	va_list ap;
	int		count = 0;

	va_start(ap, str);
	while (*str)
	{
		if (*str != '%')
			count += write(1, str, 1);
		else
		{
			str++;
			if (*str == 's')
				print_str(va_arg(ap, char*), &count);
			if (*str == 'd')
				print_base((long long int)va_arg(ap, int), 10, &count);
			if (*str == 'x')
				print_base((long long int)va_arg(ap, unsigned int), 16, &count);
			if (*str == '%')
				count += write(1, str, 1);
		}
		str++;
	}
	return(count);
}