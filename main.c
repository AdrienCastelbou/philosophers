#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct	s_philo {
		int				id;
		pthread_mutex_t	*l_fork;
		pthread_mutex_t	*r_fork;
		int				*finish;
		struct s_philo	*next;
		struct s_philo	*prev;
}				t_philo;


long int	ft_atoli(char *s)
{
	long int result;
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
	pthread_mutex_t	fork;

	if (id > nb)
	{
		first_phil->prev = prev;
		first_phil->l_fork = prev->r_fork;
		return (first_phil);
	}
	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->id = id;
	philo->prev = prev;
	if (philo->prev)
		philo->l_fork = philo->prev->r_fork;
	philo->r_fork = &fork;
	philo->finish = first_phil->finish;
	philo->next = set_philos(philo, id + 1, nb, first_phil);
	return (philo);
}

t_philo	*set_first_philo(void)
{
	t_philo			*philo;
	pthread_mutex_t	fork;
	int				is_finish;

	if (!(philo = malloc(sizeof(t_philo))))
		return (NULL);
	philo->id = 1;
	philo->prev = NULL;
	philo->r_fork = &fork;
	philo->next = NULL;
	is_finish = 0;
	philo->finish = &is_finish;
	return (philo);
}

int	main(int ac, char **av)
{
	int				philos_nb;
	t_philo			*philo;

	if (ac < 2)
		return (1);
	philos_nb = (int) ft_atoli(av[1]);
	philo = set_first_philo();
	philo->next = set_philos(philo, 2, philos_nb, philo);
	int one;
	t_philo *current;

	current = philo;
	one = 0;
	while (current->id != 1 || one == 0)
	{
		one = 1;
		printf("For %d\n", current->id);
		if (current->l_fork == current->prev->r_fork && current->r_fork == current->next->l_fork)
			printf("Forks are good shared\n");
		else
			printf("Bad share\n");
		printf("-------\n");
		current = current->next;
	}
}
