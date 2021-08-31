/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 10:29:54 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/31 10:11:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philos(t_philo *philo, int philos_nb)
{
	t_philo	*next;
	int		i;

	if (!philo)
		return ;
	pthread_mutex_destroy(philo->write);
	free(philo->write);
	free(philo->finish);
	free(philo->must_eat);
	free(philo->t_start);
	pthread_mutex_destroy(philo->check_end);
	free(philo->check_end);
	i = -1;
	while (++i < philos_nb && philo)
	{
		next = philo->next;
		pthread_mutex_destroy(philo->r_fork);
		free(philo->r_fork);
		pthread_mutex_destroy(&philo->check_death);
		free(philo);
		philo = NULL;
		philo = next;
	}
}

void	ft_free(t_philo *philo, pthread_t *threads, int philos_nb)
{
	free_philos(philo, philos_nb);
	if (threads)
		free(threads);
}
