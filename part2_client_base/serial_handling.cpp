#include "serial_handling.h"
char buffer[30];
extern shared_vars shared;
bool wait_on_serial(uint8_t nbytes, long timeout);
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
long long longlong_from_serial() {
    long long array[15];
    long long temp;
    int i = 0;
    int flag = 1;
    long long sum = 0;
    char current;
    while(true)
    {
      current = Serial.read();
      if(current != ' ' && current != '\n' && current != '-' && current != -1 && current > 47 && current < 58)
      {
        array[i] = (current - '0');
        i++;
      }
      else if(current == '-')
      {
        flag = -1;
      }
      else if(current == -1)
      {
        bool bytesarrived = wait_on_serial((uint8_t)1, (long)1000);
        if(!bytesarrived)
        {
          break;
        }
      }
      else
      {
        break;
      }
    }
    for(int j =0; j < i; j++)
    {
      temp = pow(10, i - j - 1);
      sum = sum + (array[j] * temp);
    }
    return sum * flag;
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
bool wait_on_serial(uint8_t nbytes, long timeout){
  unsigned long deadline = millis() + timeout;
  while (Serial.available()<nbytes && (timeout<0 || millis()<deadline))
  {
    delay(1);
  }
  return Serial.available() >= nbytes;
}
uint8_t get_waypoints(const lon_lat_32& start, const lon_lat_32& end) {
  // Currently this does not communicate over the serial port.
  // It just stores a path length of 0. You should make it communicate with
  // the server over Serial using the protocol in the assignment description
  // TODO: implement the communication protocol from the assignment
  // for now, nothing is stored
  char myString[3] = "R ";
  char num1[15];
  char num2[15];
  char num3[15];
  char num4[15];
  int size1 = 0;
  int size2 = 0;
  int size3 = 0;
  int size4 = 0;
  longlongtochar(num1, start.lat, size1);
  longlongtochar(num2, start.lon, size2);
  longlongtochar(num3, end.lat, size3);
  longlongtochar(num4, end.lon, size4);
  Serial.print(myString);
  for(int i = 0; i < size1; i++)
  {
    Serial.write(num1[i]);
  }
  Serial.write(" ");
  for(int i = 0; i < size2; i++)
  {
    Serial.write(num2[i]);
  }
  Serial.write(" ");
  for(int i = 0; i < size3; i++)
  {
    Serial.write(num3[i]);
  }
  Serial.write(" ");
  for(int i = 0; i < size4; i++)
  {
    Serial.write(num4[i]);
  }
  Serial.write('\n');
  char in_char;
  char space;
  char endline;
  int i = 0;
  bool bytesarrived;
  int gotNumPoints = 0;
  while(true)
  {
      // read the incoming byte:
    if(gotNumPoints == 0)
    {
      bytesarrived = wait_on_serial((uint8_t)3, (long)10000);
      if(bytesarrived)
      {
        char in_char = Serial.read();
        shared.num_waypoints = in_char;
        if(in_char == 'N')
        {
          Serial.read();
          shared.num_waypoints = longlong_from_serial();
          if(shared.num_waypoints == 0){
            return 0;
          }
        }
        Serial.print("A\n");
        gotNumPoints = 1;
      }
      else
      {
        return 0;
      }
    }
    else if(gotNumPoints == 1)
    {
      bytesarrived = wait_on_serial((uint8_t)1, (long)1000);
      if(bytesarrived)
      {
        char in_char = Serial.read();
        if(in_char == 'W')
        {
          bytesarrived = wait_on_serial((uint8_t)1, (long)1000);
          space = Serial.read();
          bytesarrived = wait_on_serial((uint8_t)1, (long)1000);
          space = Serial.read();
          bytesarrived = wait_on_serial((uint8_t)7, (long)1000);
          if(bytesarrived)
          {
            shared.waypoints[i].lat = longlong_from_serial();
          }
          bytesarrived = wait_on_serial((uint8_t)8, (long)1000);
          if(bytesarrived)
          {
            shared.waypoints[i].lon = longlong_from_serial();
          }
          i++;
          Serial.print("A\n");
        }
        else if(in_char == 'E')
        {
          bytesarrived = wait_on_serial((uint8_t)1, (long)1000);
          space = Serial.read();
          break;
        }
      }
      else
      {
        return 0;
      }
    }
  }

  // 1 indicates a successful exchange, of course you should only output 1
  // in your final solution if the exchange was indeed successful
  // (otherwise, return 0 if the communication failed)
  return 1;
}
