#include <stdio.h>

const char s[] = "1+(1+2*2)*2+(8+1)$";
int p = 0;

int GetPenis ();
int GetG ();
int GetN ();
int GetE ();
int GetT ();
int SyntaxError ();

int main ()
{
    printf ("%d\n", GetG());

    return 0;
}

int GetPenis ()
{
    if (s[p] == '(')
    {
        p++;
        int val = GetE ();
        if (s[p] != ')')
        {
            SyntaxError ();
        }
        p++;
        return val;
    }
    else
        return GetN ();
}

int GetG ()
{
    int val = GetE ();
    if (s[p] != '$')
        SyntaxError ();
    return val;
}

int GetN ()
{
    int val = 0;
    int pOld = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val*10 + s[p] - '0';
        p++;
    }

    if (pOld == p)
        SyntaxError ();

    return val;
}

int GetE ()
{
    int val = GetT ();
    while (s[p] == '+' || s[p] == '-')
    {
        int op = s[p];
        p++;
        int val2 = GetT ();
        if (op == '+')
            val += val2;
        else val -= val2;
    }
    return val;
}

int GetT ()
{
    int val = GetPenis ();
    //int val = 0;
    while (s[p] == '*' || s[p] == '/')
    {
        int op = s[p];
        p++;
        int val2 = GetPenis ();
        if (op == '*')
            val = val2 * val;
        else val = val / val2;
    }
    return val;
}

int SyntaxError ()
{
    printf ("%c", s[p]);
    printf ("Syntax error\n");

    return 0;
} 