/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/30 10:25:00 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/30 10:26:08 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	write_step(t_philo *philo, char *str)
{
	long long int	time;

	pthread_mutex_lock(philo->write);
	if (check_end(philo))
	{
		pthread_mutex_unlock(philo->write);
		return (0);
	}
	time = get_time() - *philo->t_start;
	ft_putnbr(time);
	ft_putchar(' ');
	ft_putnbr(philo->id);
	ft_putstr(str);
	pthread_mutex_unlock(philo->write);
	return (1);
}

void	philo_routine(t_philo *philo,
		pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	write_step(philo, " has taken a fork\n");
	pthread_mutex_lock(second_fork);
	write_step(philo, " has taken a fork\n");
	write_step(philo, " is eating\n");
	pthread_mutex_lock(&philo->check_death);
	philo->t_satiate = get_time() - *philo->t_start;
	pthread_mutex_unlock(&philo->check_death);
	ft_usleep(philo->t_eat * 1000);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_lock(philo->check_end);
	if (*philo->must_eat != -1)
		philo->t_must_eat -= 1;
	pthread_mutex_unlock(philo->check_end);
	write_step(philo, " is sleeping\n");
	ft_usleep(philo->t_sleep * 1000);
	write_step(philo, " is thinking\n");
}

void	set_philo_finish_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->check_end);
	*philo->must_eat -= 1;
	pthread_mutex_unlock(philo->check_end);
}

void	*run_philo(void *v_philo)
{
	t_philo			*philo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	philo = v_philo;
	write_step(philo, " is thinking\n");
	if (philo->id % 2 != 0)
	{
		first_fork = philo->r_fork;
		second_fork = philo->l_fork;
	}
	else
	{
		ft_usleep(1000);
		first_fork = philo->l_fork;
		second_fork = philo->r_fork;
	}
	while (check_end(philo) == 0)
		philo_routine(philo, first_fork, second_fork);
	if (philo->t_must_eat == 0)
		set_philo_finish_eat(philo);
	return (NULL);
}

void	*run_one_philo(void	*v_philo)
{
	t_philo			*philo;

	philo = v_philo;
	write_step(philo, " is thinking\n");
	if (check_end(philo) == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		write_step(philo, " has taken a fork\n");
		ft_usleep((philo->t_die + 1) * 1000);
		pthread_mutex_unlock(philo->r_fork);
	}
	if (philo->t_must_eat == 0)
	{
		pthread_mutex_lock(philo->check_end);
		*philo->must_eat -= 1;
		pthread_mutex_unlock(philo->check_end);
	}
	return (NULL);
}
