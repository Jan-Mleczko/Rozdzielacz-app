void	putstr (char *s)
{	char	c;
	while (c = *s++)
		putchar (c);
}
void	newln ()
{	putchar ('\n');
}
void	putln (char *s)
{	putstr (s);
	newln ();
}
void	putnum (int n)
{	int	q;
	if (n < 0)
	{	putchar ('-');
		putnum (-n);
	}
	else
	{	if (q = n / 10)
			putnum (q);
		putchar ('0' + n % 10);
	}
}
void	waitln ()
{	while (getchar() != '\n');
}
void	getln (char *s, int l)
{	char	c;
	while ((c = getchar ()) != '\n')
		if (l)
		{	*s++ = c;
			--l;
		}
	*s = 0;
}
int	getnum ()
{	char	c;
	int	val = 0;
	while ((c = getchar ()) != '\n')
		if (c >= '0' && c <= '9')
			val = c - '0' + (val<<3) + (val<<1);
	return val;
}
void	mwait (char *msg)
{	putstr (msg);
	putstr ("... ");
	waitln ();
}
void	mgetl (char *msg, char *s, int l)
{	putstr (msg);
	putstr ("? ");
	getln (s, l);
}
int	mgetn (char *msg)
{	putstr (msg);
	putstr ("? ");
	return getnum ();
}
