/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 10:29:54 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/30 16:12:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philos(t_philo *philo, int philos_nb)
{
	t_philo	*next;
	int		i;

	i = 0;
	while (i < philos_nb && philo)
	{
		next = philo->next;
		free(philo->r_fork);
		if (philo->id == 1)
		{
			free(philo->write);
			free(philo->finish);
			free(philo->must_eat);
			free(philo->t_start);
			free(philo->check_end);
		}
		pthread_mutex_destroy(&philo->check_death);
		philo->r_fork = NULL;
		free(philo);
		philo = NULL;
		philo = next;
		i++;
	}
}

void	ft_free(t_philo *philo, pthread_t *threads, int philos_nb)
{
	free_philos(philo, philos_nb);
	if (threads)
		free(threads);
}
