/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_philos.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 10:22:07 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/30 10:22:50 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*set_philos2(t_philo *philo, t_philo *first_phil)
{
	philo->r_fork = malloc(sizeof(pthread_mutex_t));
	if (!(philo->r_fork))
	{
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->r_fork, NULL);
	philo->finish = first_phil->finish;
	philo->t_start = first_phil->t_start;
	philo->t_die = first_phil->t_die;
	philo->t_eat = first_phil->t_eat;
	philo->t_sleep = first_phil->t_sleep;
	philo->must_eat = first_phil->must_eat;
	philo->t_must_eat = first_phil->t_must_eat;
	return (philo);
}

t_philo	*set_philos(t_philo *prev, int id, int nb, t_philo *first_phil)
{
	t_philo			*philo;

	if (id > nb)
	{
		first_phil->prev = prev;
		first_phil->l_fork = prev->r_fork;
		return (first_phil);
	}
	philo = malloc(sizeof(t_philo));
	if (!(philo))
		return (NULL);
	*first_phil->finish = 0;
	philo->id = id;
	philo->prev = prev;
	if (philo->prev)
		philo->l_fork = philo->prev->r_fork;
	philo->write = first_phil->write;
	philo->check_end = first_phil->check_end;
	pthread_mutex_init(&philo->check_death, NULL);
	if (!set_philos2(philo, first_phil))
		return (NULL);
	philo->next = set_philos(philo, id + 1, nb, first_phil);
	return (philo);
}

t_philo	*set_first_philo3(t_philo *philo, int nb_params,
		char **params, int philos_nb)
{
	*philo->must_eat = -1;
	philo->t_must_eat = 1;
	philo->t_die = ft_atolli(params[0]);
	philo->t_eat = ft_atolli(params[1]);
	philo->t_sleep = ft_atolli(params[2]);
	if (nb_params == 6)
	{
		philo->t_must_eat = ft_atolli(params[3]);
		*philo->must_eat = philos_nb;
	}
	return (philo);
}

t_philo	*set_first_philo2(t_philo *philo, int nb_params,
		char **params, int philos_nb)
{
	philo->check_end = malloc(sizeof(pthread_mutex_t));
	if (!philo->check_end)
	{
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->check_end, NULL);
	pthread_mutex_init(&philo->check_death, NULL);
	philo->next = NULL;
	philo->finish = malloc(sizeof(int));
	if (!philo->finish)
		return (NULL);
	*philo->finish = 0;
	philo->t_start = malloc(sizeof(long long int));
	if (!philo->t_start)
		return (NULL);
	philo->must_eat = malloc(sizeof(int));
	if (!philo->must_eat)
		return (NULL);
	set_first_philo3(philo, nb_params, params, philos_nb);
	return (philo);
}

t_philo	*set_first_philo(int nb_params, char **params, int	philos_nb)
{
	t_philo			*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = 1;
	philo->prev = NULL;
	philo->r_fork = malloc(sizeof(pthread_mutex_t));
	if (!philo->r_fork)
	{
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->r_fork, NULL);
	philo->write = malloc(sizeof(pthread_mutex_t));
	if (!philo->write)
	{
		free(philo->r_fork);
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->write, NULL);
	set_first_philo2(philo, nb_params, params, philos_nb);
	return (philo);
}
