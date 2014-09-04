/* writetoserial.c
 *
 * Copyright (C) 2014 Anders Skoglund
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

const char *NAME = "writetoserial";

void print_help();

int main(int argc, char **argv){
  const char *baud_rate;
  int input_file_name;
  int output_file_name;
  char c;
  
  /* Use 9600 as default value for the baud rate */
  baud_rate = "9600";

  while ((c = getopt (argc, argv, "b:h")) != -1)
    {
      switch (c)
	{
	case 'b':
	  baud_rate = optarg;;
	  break;
	case 'h':
	  print_help();
	  return 0;
	  break;
	case '?':
	  if (optopt == 'b')
	    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
	  else if (isprint (optopt))
	    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	  else
	    fprintf (stderr, "Unknown option character `\\x%x'.\n",	optopt);
	  print_help();
	  return 1;
	default:
	  fprintf (stderr, "Bad state reached.\n");
	  return 1;
	}
    }
  
  if(strlen(baud_rate) > 6)
    {
      fprintf (stderr, "Baud rate must be at most 6 digits long\n");
      return 1;
    }
  
  unsigned int i;
  for(i = 0; i < strlen(baud_rate); ++i)
    {
      if(!isdigit(baud_rate[i]))
	{
	  fprintf (stderr, "Baud rate is not a number\n");
	  return 1;
	}
    }

  int index = optind;
  
  if(index != argc - 2)
    {
      fprintf(stderr, "Incorrect number of arguments\n");
      return 1;
    }

  input_file_name = index;
  output_file_name = index + 1;
  
  FILE *in = fopen(argv[input_file_name], "r");
  if(in == NULL)
    {
      fprintf(stderr, "Could not open file %s\n", argv[input_file_name]);
      return 1;
    }
  
  FILE *out = fopen(argv[output_file_name], "w");
  if(out == NULL)
    {
      fprintf(stderr, "Could not open file %s\n", argv[output_file_name]);
      return 1;
    }

  /* Use stty to set serial port settings. */
  char *system_command = (char*)malloc(20 + strlen(argv[output_file_name]) + strlen(baud_rate));
  strcpy (system_command, "stty -F ");
  strcat (system_command, argv[output_file_name]);
  strcat (system_command, " raw -echo ");
  strcat (system_command, baud_rate);
  
  if(system(system_command) != 0)
    {
      fprintf(stderr, "Could not set serial port options\n");
      return 1;
    }
  
  free(system_command);

  /* Wait a little to give the system time to set things up before sending data */
  sleep(2);

  unsigned char buffer;
	
  /**
   * Read one byte from the file then write it to the serial device.
   * Repeat until EOF is reached.
   */
  while(fread(&buffer, 1, 1, in))
    {
      fwrite(&buffer, 1, 1, out);
    }

  /* Wait a moment to let it finish writing buffered data. */
  sleep(2);
  
  fclose(in);
  fclose(out);

  return 0;
}

void print_help()
{
  printf("Usage: %s [OPTION] INPUT_FILE SERIAL_DEVICE\n", NAME);
  printf("Writes the contents of a file to a serial port device as binary data.\n");
  printf("  -h\tPrint usage information\n");
  printf("  -b\tSet baud rate (default 9600)\n");
}
