#include <iostream>
using namespace std;
long long pow(int base, int exponent)
{
  long long copy = (long long)base;
  for(int i = 1; i <exponent; i++)
  {
    copy = copy * base;
  }
  if(exponent == 0)
  {
    copy = 1;
  }
  return copy;
}

void longlongtochar(char* myString, long long num, int &size) {
    long long numcpy = num;
    long long temp, temp2, temp3;
    bool neg = false;
    int digit;
    int delim = 0;
    while(num != 0)
    {
      num = num /10;
      size++;
    }
    if(numcpy < 0)
    {
      myString[0] = '-';
      delim = 1;
      numcpy = numcpy * -1;
      neg = true;
    }
    for(int i = size-1; i >= 0; i--)
    {
      temp = pow(10, i);
      temp2 = numcpy / (temp);
      temp3 = numcpy / (((temp) * 10));
      digit = temp2 - (temp3*10);
      //Serial.println((int)temp2);
      //Serial.println((int)digit);
      myString[(size-1) - i + delim] = digit + '0';
    }
    if(neg)
    {
      size++;
    }
   
}
int main()
{
  char num1[15];
  int size1 = 0;
  long long num =52354678;
  longlongtochar(num1, num, size1);
  for(int i = 0; i < size1; i++)
  {
  	cout << num1[i];
  }
}