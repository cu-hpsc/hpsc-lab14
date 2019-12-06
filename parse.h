#include <argp.h>

/* This structure is used by main to communicate with parse_opt. */
struct args
{
  long array_len;
  int threads;
  int reps;
  int verbosity;
};

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] =
{
  {"array_len",'a',"count",0,"the number of complex data elements"},
  {"threads",'t',"count",0,"the number of threads (0=> OMP default)"},
  {"reps",'r',"count",0,"repetitions over which to time"},
  {"verbosity",'v',"count",0,"non-zero to print results for visualization"},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct args *args = state->input;
  switch (key)
    {
    case ARGP_KEY_INIT:
      args->array_len = 1 << 10;
      args->threads = 0;
      args->reps = 1;
      args->verbosity = 0;
      break;
    case 'a':
      args->array_len = strtol(arg,NULL,10);
      break;
    case 't':
      args->threads = strtol(arg,NULL,10);
      break;
    case 'r':
      args->reps = strtol(arg,NULL,10);
      break;
    case 'v':
      args->verbosity = strtol(arg,NULL,10);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

void parse_args(struct args *args, int argc, char **argv)
{
  static struct argp argp = {options, parse_opt, NULL, NULL};
  argp_parse(&argp, argc, argv, 0, 0, args);
  return;
}
