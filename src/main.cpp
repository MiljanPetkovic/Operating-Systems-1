#include "System.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {

	System::initialize();

	int res = userMain(argc, argv);

	System::restore();

	return res;

}
