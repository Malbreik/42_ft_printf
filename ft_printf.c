/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malbreik <malbreik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:12:47 by malbreik          #+#    #+#             */
/*   Updated: 2025/01/15 12:04:57 by malbreik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *string, ...)
{
	va_list	flags;
	int		i;
	int		total_chars;

	i = 0;
	total_chars = 0;
	va_start(flags, string);
	while (string[i] != '\0')
	{
		if (string[i] == '%')
		{
			i++;
			if (ft_printf_output(flags, string, i, &total_chars) == -1)
				return (-1);
		}
		else
		{
			if (ft_putchar(string[i]) == -1)
				return (-1);
			total_chars++;
		}
		i++;
	}
	va_end(flags);
	return (total_chars);
}

int	ft_printf_output(va_list flags, const char *string, int i, int *total)
{
	int	printed;

	printed = 0;
	if (string[i] == 'c')
		printed = ft_putchar(va_arg(flags, int));
	else if (string[i] == 's')
		printed = ft_putstr(va_arg(flags, char *));
	else if (string[i] == 'd' || string[i] == 'i')
		printed = ft_putnbr(va_arg(flags, int));
	else if (string[i] == 'u')
		printed = ft_putnbr_u(va_arg(flags, unsigned int));
	else if (string[i] == 'x' || string[i] == 'X')
		printed = ft_puthex(va_arg(flags, unsigned int), string[i], total);
	else if (string[i] == 'p')
		printed = ft_putmem(va_arg(flags, size_t), total);
	else if (string[i] == '%')
		printed = ft_putchar('%');
	if (printed == -1)
		return (-1);
	if (string[i] != 'x' || string[i] != 'X')
		*total += printed;
	return (0);
}
