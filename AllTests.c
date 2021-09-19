#include <stdio.h>
#include <string.h>
#include "CuTest.h"
CuSuite* TransformGetSuite();
CuSuite* HandleSignsGetSuite();
CuSuite* BranchGetSuite();
CuSuite* SwitchGetSuite();/////////////

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

 	CuSuiteAddSuite(suite, TransformGetSuite());
 	CuSuiteAddSuite(suite, HandleSignsGetSuite());
 	CuSuiteAddSuite(suite, BranchGetSuite());
 	CuSuiteAddSuite(suite, SwitchGetSuite());////////////
 	
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
}
