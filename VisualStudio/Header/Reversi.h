#ifdef REVERSI_EXPORTS
#define REVERSI_API __declspec(dllexport)
#else
#define REVERSI_API __declspec(dllimport)
#endif

#include <string>
#include "header.h"

extern "C" {
	REVERSI_API char * communication(char *str);
}
