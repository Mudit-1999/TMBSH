SOURCES.c= shell.c utility/print_prompt.c utility/input_command.c utility/parse.c utility/buildin_command.c utility/execute.c utility/pinfo_cmd.c utility/process_launch.c utility/bonus.c utility/check_pipe.c utility/user_defined.c utility/job_control.c
INCLUDES=
INCLUDES=
CFLAGS=
SLIBS=
PROGRAM= TMBSH

OBJECTS= $(SOURCES.c:.c=.o)

.KEEP_STATE:

debug := CFLAGS= -g

all debug: $(PROGRAM)

$(PROGRAM): $(INCLUDES) $(OBJECTS)
	$(LINK.c) -o $@ $(OBJECTS) $(SLIBS)

clean:
	rm -f $(PROGRAM) $(OBJECTS)
