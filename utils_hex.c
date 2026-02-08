/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_hex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malbreik <malbreik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 22:14:54 by malbreik          #+#    #+#             */
/*   Updated: 2025/01/15 11:46:26 by malbreik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*check_base(char c)
{
	if (c == 'x')
		return ("0123456789abcdef");
	else
		return ("0123456789ABCDEF");
}

int	ft_puthex(unsigned int num, char c, int *printed)
{
	char			*base;
	int				i;
	char			string[25];

	i = 0;
	base = check_base(c);
	if (num == 0)
	{
		if (ft_putchar('0') == -1)
			return (-1);
		return (1);
	}
	while (num != 0)
	{
		string[i++] = base [num % 16];
		num /= 16;
	}
	*printed += i;
	while (i--)
	{
		if (ft_printf("%c", string[i]) == -1)
			return (-1);
	}
	return (0);
}

static int	check_zero_mem(size_t num, int *p)
{
	if (num == 0)
	{
		if (ft_printf("0") == -1)
			return (-1);
		*p += 1;
	}
	return (0);
}

int	ft_putmem(size_t num, int *printed)
{
	char			*base;
	int				i;
	char			string[25];

	i = 0;
	base = "0123456789abcdef";
	if (ft_printf("0x") == -1)
		return (-1);
	*printed += 2;
	if (check_zero_mem(num, printed) == -1)
		return (-1);
	while (num != 0)
	{
		string[i++] = base [num % 16];
		num /= 16;
		(*printed)++;
	}
	while (i--)
	{
		if (ft_putchar(string[i]) == -1)
			return (-1);
	}
	return (0);
}
