/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 10:55:44 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/31 10:56:16 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long int	time_to_usleep(t_philo *philo, long long int t_usleep)
{
	long long int	time;

	time = get_time();
	if (philo->t_satiate + philo->t_die < time - *philo->t_start + t_usleep)
		return ((philo->t_die + 1) * 1000);
	return (t_usleep * 1000);
}

void	ft_usleep(long long int delay)
{
	long long int	goal;

	goal = get_time() + (delay / 1000);
	while (get_time() < goal)
		usleep(100);
}
