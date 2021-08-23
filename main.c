#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
typedef struct	s_philo {
		int				id;
		pthread_mutex_t	*l_fork;
		pthread_mutex_t	*r_fork;
		pthread_mutex_t	*write;
		long long int	t_die;
		long long int	t_eat;
		long long int	t_sleep;
		long long int	t_satiate;
		int				must_eat;
		int				t_must_eat;
		int				*finish;
		struct s_philo	*next;
		struct s_philo	*prev;
}				t_philo;


long int	ft_atolli(char *s)
{
	long long int result;
	int		i;
	int		sign;

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
	while (s[i] >= '0' && s[i] <='9')
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
	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	*first_phil->finish = 0;
	philo->id = id;
	philo->prev = prev;
	if (philo->prev)
		philo->l_fork = philo->prev->r_fork;
	philo->write = first_phil->write;
	if (!(philo->r_fork = malloc(sizeof(pthread_mutex_t))))
	{
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->r_fork, NULL);
	philo->finish = first_phil->finish;
	philo->t_die = first_phil->t_die;
	philo->t_eat = first_phil->t_eat;
	philo->t_sleep = first_phil->t_sleep;
	philo->must_eat = first_phil->must_eat;
	philo->t_must_eat = first_phil->t_must_eat;
	philo->next = set_philos(philo, id + 1, nb, first_phil);
	return (philo);
}

t_philo	*set_first_philo(int nb_params, char **params)
{
	t_philo			*philo;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->id = 1;
	philo->prev = NULL;
	if (!(philo->r_fork = malloc(sizeof(pthread_mutex_t))))
	{
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->r_fork, NULL);
	if (!(philo->write = malloc(sizeof(pthread_mutex_t))))
	{
		free(philo->r_fork);
		free(philo);
		return (NULL);
	}
	pthread_mutex_init(philo->write, NULL);
	philo->next = NULL;
	philo->must_eat = 0;
	if (!(philo->finish = malloc(sizeof(int))))
		return (NULL);
	*philo->finish = 0;
	philo->t_must_eat = 1;
	philo->t_die = ft_atolli(params[0]);
	philo->t_eat = ft_atolli(params[1]);
	philo->t_sleep = ft_atolli(params[2]);
	if (nb_params == 6)
	{
		philo->t_must_eat = ft_atolli(params[3]);
		philo->must_eat = 1;
	}
	return (philo);
}

void	free_philos(t_philo *philo, int philos_nb)
{
	t_philo	*next;
	int i;

	i = 0;
	while (i < philos_nb)
	{
		next = philo->next;
		free(philo->r_fork);
		if (philo->id == 1)
		{
			free(philo->write);
			free(philo->finish);
		}
		philo->r_fork = NULL;
		free(philo);
		philo = NULL;
		philo = next;
		i++;
	}
}

long long int	get_time()
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

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_putstr(char *s)
{
	write(1, s, ft_strlen(s));
}

int		write_step(t_philo *philo, char *str)
{
	long long int time;

	pthread_mutex_lock(philo->write);
	time = get_time();
	if (*philo->finish == 1)
	{
		pthread_mutex_unlock(philo->write);
		return (0);
	}
	ft_putnbr(time);
	ft_putchar(' ');
	ft_putnbr(philo->id);
	if (time - philo->t_satiate >= philo->t_die)
	{
		ft_putstr(" died\n");
		*philo->finish = 1;
		pthread_mutex_unlock(philo->write);
		return (0);
	}
	else
		ft_putstr(str);
	pthread_mutex_unlock(philo->write);
	return (1);
}

void	*run_philo(void *v_philo)
{
	t_philo	*philo;
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	philo = v_philo;
	if (philo->id % 2 != 0)
	{
		first_fork = philo->r_fork;
		second_fork = philo->l_fork;
	}
	else
	{
		first_fork = philo->l_fork;
		second_fork = philo->r_fork;
	}
	philo->t_satiate = get_time();
	while (*philo->finish == 0 && philo->t_must_eat)
	{
		pthread_mutex_lock(first_fork);
		if (write_step(philo, " has taken a fork\n") == 0)
		{
			pthread_mutex_unlock(first_fork);
			return (NULL);
		}
		pthread_mutex_lock(second_fork);
		if (write_step(philo, " has taken a fork\n") == 0)
		{
			pthread_mutex_unlock(first_fork);
			pthread_mutex_unlock(second_fork);
			return (NULL);
		}
		write_step(philo, " is eating\n");
		philo->t_satiate = get_time();
		usleep(philo->t_eat * 1000);
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		if (philo->must_eat)
			philo->t_must_eat -= 1;
		if (write_step(philo, " is sleeping\n") == 0)
			return (NULL);
		usleep(philo->t_sleep * 1000);
		if (write_step(philo, " is thinking\n") == 0)
			return (NULL);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	int				philos_nb;
	t_philo			*philo;
	pthread_t		*threads;
	int				i;

	if (ac < 5 || ac > 6)
		return (1);
	philos_nb = (int) ft_atolli(av[1]);
	philo = set_first_philo(ac, &av[2]);
	philo->next = set_philos(philo, 2, philos_nb, philo);
	if (!(threads = malloc(sizeof(pthread_t) * philos_nb)))
		return (1);
	i = -1;
	while (++i < philos_nb)
	{
		pthread_create(&threads[i], NULL, &run_philo, philo);
		philo = philo->next;
	}
	i = -1;
	while (++i < philos_nb)
		pthread_join(threads[i], NULL);
	free_philos(philo, philos_nb);
	free(threads);
}
