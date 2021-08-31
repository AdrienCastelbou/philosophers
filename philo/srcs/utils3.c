/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/31 10:55:44 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/31 17:14:12 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	launch_philos_thread(t_philo *philo, pthread_t *threads, int philos_nb)
{
	int	i;

	i = -1;
	while (++i < philos_nb)
	{
		philo->t_satiate = get_time() - *philo->t_start;
		pthread_create(&threads[i], NULL, &run_philo, philo);
		philo = philo->next;
	}
}

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
