/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 10:28:06 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/30 10:28:57 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long int	get_time(void)
{
	struct timeval	current_time;
	long long int	time;

	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (time);
}

void	ft_usleep(long long int delay)
{
	long long int	goal;

	goal = get_time() + (delay / 1000);
	while (get_time() < goal)
		usleep(100);
}

int	check_args(char **args)
{
	int	i;
	int	j;

	i = -1;
	while (args[++i])
	{
		j = -1;
		while (args[i][++j] == '-' || args[i][j] == '+')
			;
		while (args[i][j] >= '0' && args[i][j] <= '9')
			j++;
		if (args[i][j])
			return (0);
	}
	return (1);
}

int	check_params(t_philo *philo, int nb)
{
	if (nb < 0 || philo->t_die < 0 || philo->t_eat < 0
		|| philo->t_sleep < 0 || philo->t_must_eat < 0)
		return (0);
	return (1);
}
