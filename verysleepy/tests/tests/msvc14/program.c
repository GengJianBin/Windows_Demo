int vs_test_fun()
{
	unsigned int i, n = 17;
	for (i=0; i<1000000000u; i++)
		n = n * 17 + i;
	return n;
}

int main()
{
	return vs_test_fun();
}
