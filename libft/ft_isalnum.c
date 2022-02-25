/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkenji-s </var/mail/rkenji-s>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/09 20:31:57 by rkenji-s          #+#    #+#             */
/*   Updated: 2021/09/09 20:32:00 by rkenji-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if (!((c <= 'z' && c >= 'a')
			|| (c <= 'Z' && c >= 'A')
			|| (c <= '9' && c >= '0')))
		return (0);
	return (1);
}