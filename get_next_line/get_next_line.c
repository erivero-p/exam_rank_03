#include "get_next_line.h"

size_t	ft_strlen(char *str) //mejor size para calloc
{
	size_t i = 0;
	while (str[i])
		i++;
	return(i);
}

bool ft_nlsrch(char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '\n')
			return (true);
		str++;
	}
	return (false);
}
//poniendo que calloc devuelve *char me ahorro casteos
char	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	char	*ptr;

	ptr = (char *)malloc((count * size));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < (count * size))
	{
		ptr[i] = '\0';
		i++;
	}
	return (ptr);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ptr;
	size_t	i = 0;
	size_t	j = 0;

	ptr = ft_calloc((ft_strlen(s1) + ft_strlen(s2)) + 1, sizeof(char));
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	return (free(s1), ptr);
}

static char	*get_piece(char *memo, int fd)
{
	char	*aux;
	int		ret;

	if (!memo)
		memo = ft_calloc(1, sizeof(char));
	ret = 1;
	aux = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	while (ret > 0 && !ft_nlsrch(aux))
	{
		ret = read(fd, aux, BUFFER_SIZE);
		if (ret < 0) //si read falla libero todo
			return (free(aux), free(memo), NULL);
		aux[ret] = '\0';
		memo = ft_strjoin(memo, aux);
		if (!memo)
			return (free(memo), free(aux), NULL);
	}
	return (free(aux), memo);
}

static char	*get_line(char	*memo)
{
	char	*line;
	int		i = 0;

	if (!memo[i]) //esta condición no se puede eliminar o peta jeje
		return (NULL);
	while (memo[i] && memo[i] != '\n')
		i++;
	line = ft_calloc((i + 1 + (memo[i] == '\n')), sizeof(char));
//si hay salto de línea (no estamos al final del archivo), aloja uno de más para ponerlo
	i = 0;
	while (memo[i] && memo[i] != '\n')
	{
		line[i] = memo[i];
		i++;
	}
	if (memo[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*ft_clean(char *memo)
{
	char	*new_line;
	int		i = 0;
	int		j = 0;

	while (memo[i] && memo[i] != '\n')
		i++;
	if (!memo[i]) //si al final de memo hay nulo (hemos llegado al final del archivo)
		return (free(memo), NULL); //liberamos la estática porque no la vamos a necesitar más
	new_line = ft_calloc((ft_strlen(memo) - i), sizeof(char)); //alojo para lo que queda tras '\n'
	i++; //paso el salto de línea
	while (memo[i])
		new_line[j++] = memo[i++];
	new_line[j] = 0;
	return (free(memo), new_line);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*memo;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	memo = get_piece(memo, fd);
	line = get_line(memo);
	memo = ft_clean(memo);
	return (line);
}