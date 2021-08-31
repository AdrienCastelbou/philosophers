/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 10:28:06 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/31 10:56:40 by acastelb         ###   ########.fr       */
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
	if (!philo)
		return (0);
	if (nb < 0 || philo->t_die < 0 || philo->t_eat < 0
		|| philo->t_sleep < 0 || philo->t_must_eat < 0)
		return (0);
	return (1);
}

int	check_philos(t_philo *philo, int nb_philos)
{
	if (!philo || !philo->r_fork || !philo->write
		|| !philo->check_end || !philo->finish
		|| !philo->t_start || !philo->must_eat)
		return (0);
	if (nb_philos > 1)
		return (1);
	philo = philo->next;
	while (philo && philo->id != 1)
	{
		if (!philo->next || !philo->r_fork)
			return (0);
	}
	if (!philo)
		return (0);
	return (1);
}
