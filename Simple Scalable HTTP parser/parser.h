
#ifndef _PARSER_H_
#define _PARSER_H_

#define DEFAULT_WEBSERVER_PORT      (80)
#define NUM_METHODS                 ((sizeof(supported_methods)) / (sizeof(method_string)))
#define BUFSIZE                     (1024)

typedef enum _parse_states {HOST, PORT, PATH, ERROR} parse_states;
typedef enum _parse_error {PERR_NONE = 0, PERR_NOT_IMPLEMENTED, PERR_BAD_REQUEST, PERR_INTERNAL_ERROR} parse_error;

typedef struct _parsed_url {
  char *host, *relpath, *orig_uri;
  int port;
} parsed_url;

int parse_url(const char *, parsed_url **);
#endif
