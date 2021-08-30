/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:36:56 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/30 10:26:51 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philos(t_philo *philo, int philos_nb)
{
	t_philo	*next;
	int		i;

	i = 0;
	while (i < philos_nb)
	{
		next = philo->next;
		free(philo->r_fork);
		if (philo->id == 1)
		{
			free(philo->write);
			free(philo->finish);
			free(philo->t_start);
		}
		pthread_mutex_destroy(&philo->check_death);
		pthread_mutex_destroy(philo->r_fork);
		philo->r_fork = NULL;
		free(philo);
		philo = NULL;
		philo = next;
		i++;
	}
}

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

int	check_end(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(philo->check_end);
	if (philo->t_must_eat && *philo->finish == 0)
		result = 0;
	else
		result = 1;
	pthread_mutex_unlock(philo->check_end);
	return (result);
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

int	check_end_routine(t_philo *philo,
		long long int start, long long int time)
{
	pthread_mutex_lock(philo->check_end);
	if (*philo->must_eat == 0)
	{
		pthread_mutex_unlock(philo->check_end);
		return (1);
	}
	pthread_mutex_unlock(philo->check_end);
	time = get_time() - start;
	pthread_mutex_lock(&philo->check_death);
	if (time - philo->t_satiate > philo->t_die)
	{
		write_step(philo, " died\n");
		pthread_mutex_lock(philo->check_end);
		*philo->finish = 1;
		pthread_mutex_unlock(philo->check_end);
		pthread_mutex_unlock(&philo->check_death);
		return (1);
	}
	pthread_mutex_unlock(&philo->check_death);
	return (0);
}

void	*monitoring(void	*v_philo)
{
	t_philo			*philo;
	long long int	time;
	long long int	start;

	philo = v_philo;
	start = *philo->t_start;
	while (1)
	{
		if (check_end_routine(philo, start, time))
			return (NULL);
		philo = philo->next;
	}
	return (NULL);
}

void	threads_init(t_philo *philo, pthread_t *threads, int philos_nb)
{
	int			i;
	pthread_t	monitor;

	i = -1;
	*philo->t_start = get_time();
	if (philos_nb == 1)
	{
		philo->t_satiate = get_time() - *philo->t_start;
		pthread_create(&threads[0], NULL, &run_one_philo, philo);
	}
	else
	{
		while (++i < philos_nb)
		{
			philo->t_satiate = get_time() - *philo->t_start;
			pthread_create(&threads[i], NULL, &run_philo, philo);
			philo = philo->next;
		}
	}
	pthread_create(&monitor, NULL, &monitoring, philo);
	pthread_join(monitor, NULL);
	i = -1;
	while (++i < philos_nb)
		pthread_join(threads[i], NULL);
}

void	ft_free(t_philo *philo, pthread_t *threads, int philos_nb)
{
	free_philos(philo, philos_nb);
	free(threads);
}

int	main(int ac, char **av)
{
	int				philos_nb;
	t_philo			*philo;
	pthread_t		*threads;

	if (ac < 5 || ac > 6 || check_args(&av[1]) == 0)
	{
		ft_putstr("Error: arguments are invalids\n");
		return (1);
	}
	philos_nb = (int) ft_atolli(av[1]);
	philo = set_first_philo(ac, &av[2], philos_nb);
	if (check_params(philo, philos_nb) == 0)
	{
		ft_putstr("Error: invalids params\n");
		free_philos(philo, 1);
		return (1);
	}
	philo->next = set_philos(philo, 2, philos_nb, philo);
	threads = malloc(sizeof(pthread_t) * philos_nb);
	if (!threads)
		return (1);
	threads_init(philo, threads, philos_nb);
	ft_free(philo, threads, philos_nb);
}
