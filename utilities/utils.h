#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond)			\
		do {					\
			if (!(cond)) {		\
				exit(-1);		\
			}					\
		} while (0)

#endif
