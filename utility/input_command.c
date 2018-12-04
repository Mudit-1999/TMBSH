#include "../header.h"

void input_cmd()
{
  size_t line_size=4000;
  char *line = (char *)malloc((line_size+1) * sizeof(char));
  if( line == NULL)
  {
      perror("Unable to allocate buffer");
      exit(EXIT_FAILURE);
  }
  size_t no_characters = getline(&line,&line_size,stdin);

  if (no_characters < 0)
  {
    printf("Some error encountered\n");
    exit(EXIT_FAILURE);
  }
  else
    separate(line);
}
