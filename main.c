/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acastelb <acastelb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:36:56 by acastelb          #+#    #+#             */
/*   Updated: 2021/08/30 09:09:46 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

typedef struct s_philo {
	int				id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*write;
	pthread_mutex_t	check_death;
	pthread_mutex_t	*check_end;
	long long int	t_die;
	long long int	t_eat;
	long long int	t_sleep;
	long long int	t_satiate;
	long long int	*t_start;
	int				*must_eat;
	int				t_must_eat;
	int				*finish;
	struct s_philo	*next;
	struct s_philo	*prev;
}				t_philo;

long int	ft_atolli(char *s)
{
	long long int	result;
	int				i;
	int				sign;

	result = 0;
	i = 0;
	sign = 1;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	while (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign *= -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (result * sign);
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
	philo->next = set_philos(philo, id + 1, nb, first_phil);
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

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(long long int nb)
{
	if (nb < 0)
	{
		ft_putchar('-');
		nb *= -1;
	}
	if (nb < 10 && nb >= 0)
		ft_putchar(nb + '0');
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putchar((nb % 10 + '0'));
	}
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

int	check_end(t_philo *philo);

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

void	ft_usleep(long long int delay)
{
	long long int	goal;

	goal = get_time() + (delay / 1000);
	while (get_time() < goal)
		usleep(100);
}

int		check_end(t_philo *philo)
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

void	*run_philo(void *v_philo)
{
	t_philo			*philo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	philo = v_philo;
	if (write_step(philo, " is thinking\n") == 0)
		return (NULL);
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
	if (philo->t_must_eat == 0)
	{
		pthread_mutex_lock(philo->check_end);
		*philo->must_eat -= 1;
		pthread_mutex_unlock(philo->check_end);
	}
	return (NULL);
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



void	*monitoring(void	*v_philo)
{
	t_philo			*philo;
	long long int	time;
	long long int	start;

	philo = v_philo;
	start = *philo->t_start;
	while (1)
	{
		pthread_mutex_lock(philo->check_end);
		if (*philo->must_eat == 0)
		{
			pthread_mutex_unlock(philo->check_end);
			return (NULL);
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
			return (NULL);
		}
		pthread_mutex_unlock(&philo->check_death);
		philo = philo->next;
	}
	return (NULL);
}

void	*run_one_philo(void	*v_philo)
{
	t_philo			*philo;

	philo = v_philo;
	if (write_step(philo, " is thinking\n") == 0)
		return (NULL);
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

int	main(int ac, char **av)
{
	int				philos_nb;
	t_philo			*philo;
	pthread_t		*threads;
	pthread_t		monitor;
	int				i;

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
	free_philos(philo, philos_nb);
	free(threads);
}
