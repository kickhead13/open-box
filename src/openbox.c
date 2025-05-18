#include <stdlib.h>
#include <unistd.h>

#define WRITEL(literal) write(1, (literal), sizeof((literal))-1)

#ifdef INSTALL_FOLDER
#define EXEC(app,args) execvp(app, args);
#else
#define INSTALL_FOLDER "./"
#define EXEC(app,args) execv(app, args);
#endif

size_t ostrlen(const char *str) {
	const char *s;
	for(s = str; *s; ++s);
	return (s - str);
}

void ostrcpy(char *to, const char *from){
	char *save = to;
	for (; (*to = *from) != '\0'; ++from, ++to);
}

void ostrcat(char *s, const char *append) {
	char *save = s;
	for (; *s; ++s);
	while ((*s++ = *append++) != '\0');
}

int help() {
  WRITEL("Usage: openbox [PROGRAM]\n");
  WRITEL("Set up a server-client environment for encrypting files.\n\n");
  WRITEL("PROGRAM:\n");
  WRITEL("  server      run the server app\n");
  WRITEL("  client      run the client app\n");
  WRITEL("  admin       run the admin client app\n\n");
  WRITEL("Example:\n");
  WRITEL(" $ openbox server -run -d /home/ana/openbox_server\n");
  return 0;
}

int main(int argc, char *const *restrict argv) {
  if(argc == 1) return help();

  char ** args = (char**)malloc(sizeof(char*) * (argc));
  for(size_t iter = 2; iter < argc; iter++) {
    args[iter-1] = (char*)malloc(sizeof(char) * ostrlen(argv[iter]));
    ostrcpy(args[iter-1], argv[iter]);
  }
  args[argc-1] = (char*)0;
  args[0] = (char*)malloc(sizeof(char) * 20);

  ostrcpy(args[0], "openbox_");
  ostrcat(args[0], argv[1]);
  
  char path[256];
  ostrcpy(path, INSTALL_FOLDER);
  ostrcat(path, args[0]);

  execvp(path,args);
  return -1;
}
