int vs_test_fun()
{
	int n = 17;
	for (int i=0; i<1000000000u; i++)
		n = n * 17 + i;
	return n;
}

extern(C)
void start()
{
	import core.sys.windows.winbase;
	ExitProcess(vs_test_fun);
}
