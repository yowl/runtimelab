using System;

Console.WriteLine("Hello world");

int i;
for (i = 0; i < 10; i++)
{
     var s = i.ToString("d3");
     var c = (char)('a' + i);
     Console.WriteLine(c);
     Console.WriteLine(s);
}
