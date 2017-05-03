#include "ft_db.h"
int choose_dbtab_path(t_obj *obj, char *message)
{
	char *database;

	database = search_database_list(obj->filename.db, message);
	if (ft_strcmp(database, "NO MATCH") == 0)
		return(-1);
  	obj->filename.table = ft_strjoin(obj->filename.curr_dir, "/database/tables/");
  	obj->filename.table = ft_strjoin(obj->filename.table, database);
  	obj->filename.table = ft_strjoin(obj->filename.table, ".txt");
  	obj->filename.curr_table = database;
  	obj->filename.tab_path = 1;

  	return (0);
}

int	print_tables(t_obj *obj)
{
	int		fd;
	char	*string;

	if (obj->filename.tab_path != 1)
	{
		if (choose_dbtab_path(obj, "Which DB Table WOULD YOU LIKE TO SEE?")== -1) 
			return (-1);
	}
	fd = open(obj->filename.table, O_RDONLY);
	ft_putstr("---------------\n");
	while(get_next_line(fd, &string))
	{
		ft_putstr(string);
		ft_putstr("\n");
	}
	ft_putstr("---------------");
	ft_putstr("\n");
	return(0);
}

void write_table_to_file(t_obj *obj)
{
	FILE *fptr;
	char *table_name;

   fptr = fopen(obj->filename.table, "a");
   table_name = get_answer("ENTER NAME OF NEW TABLE");
   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }
   fprintf(fptr, "%s\n", table_name);
   fclose(fptr);
}

int add_table_to_db(t_obj *obj)
{
	if (choose_dbtab_path(obj, "ENTER DB NAME THAT YOUD LIKE TO ADD TO") == -1)
	{
		ft_putstr("NO SUCH INFORMATION");
		return(-1);
	}
  	printf("%s\n", obj->filename.table);
	write_table_to_file(obj);
	print_tables(obj);
	return (0);
}


int count_tables(t_obj *obj)
{
	int fd;
	int x;
	char *line;

	x = 0;
	fd = open(obj->filename.table, O_RDONLY);
	while (get_next_line(fd, &line) == 1)
		x++;
	close(fd);
	return (x);
}

int delete_table(t_obj *obj)
{
	char *delete;
	int table_count;
	char **new;
	int x;
	int fd;
	char *line;

	x = 0;

	if (choose_dbtab_path(obj, "ENTER DB NAME THAT HAS A TABLE YOUD LIKE TO DELETE") == -1)
	{
		ft_putstr("NO SUCH INFORMATION");
		return(-1);
	}
	printf("%s\n", obj->filename.table);
	print_tables(obj);
	delete = search_database_list(obj->filename.table, "ENTER DATABASE TABLE TO BE DELETED");
	if (ft_strcmp(delete, "NO MATCH") == 0)
	{
		ft_putstr("DOESN'T MATCH");
		return (0);
	}
	printf("%s\n", delete);
	table_count = count_tables(obj);
	printf("%d\n", table_count);

	
	fd = open(obj->filename.table, O_RDONLY);
	new = (char**)malloc(sizeof(char**) * table_count);
	while (get_next_line(fd, &line) == 1)
	{
		if (ft_strcmp(delete, line) != 0)
		{
			new[x] = (char*)malloc(sizeof(char *) * ft_strlen(line) + 1);
			ft_strcpy(new[x], line);
			printf("%s\n", new[x]);
			x++;
		}
	}
	overwrite_db(obj->filename.table, new, x);
	return (1);
}

int update_table(t_obj *obj)
{
	char *delete;
	char *update_name;
	int table_count;
	char **new;
	int x;
	int fd;
	char *line;

	x = 0;

	if (choose_dbtab_path(obj, "ENTER DB NAME THAT HAS A TABLE YOUD LIKE TO UPDATE") == -1)
	{
		ft_putstr("NO SUCH INFORMATION");
		return(-1);
	}
	print_tables(obj);
	delete = search_database_list(obj->filename.table, "ENTER DATABASE TABLE TO BE UPDATED");
	if (ft_strcmp(delete, "NO MATCH") == 0)
	{
		ft_putstr("DOESN'T MATCH");
		return (0);
	}
	update_name = get_answer("WHAT DO YOU WANT TO UPDATE THE NAME TO?");
	table_count = count_tables(obj);
	fd = open(obj->filename.table, O_RDONLY);
	new = (char**)malloc(sizeof(char**) * table_count);
	while (get_next_line(fd, &line) == 1)
	{
		if (ft_strcmp(delete, line) == 0)
		{
			new[x] = (char*)malloc(sizeof(char *) * ft_strlen(update_name) + 1);
			ft_strcpy(new[x], update_name);
			printf("%s\n", new[x]);
			x++;
		}
		else
		{
			new[x] = (char*)malloc(sizeof(char *) * ft_strlen(line) + 1);
			ft_strcpy(new[x], line);
			printf("%s\n", new[x]);
			x++;
		}
	}
	overwrite_db(obj->filename.table, new, x);
	return (0);
}


