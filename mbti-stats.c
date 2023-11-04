#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

/*
This program opens a file containing statistics for the MBTI population in the USA. It then reads in each line, which is a float corresponding to each MBTI type. There is a statistical analysis that is done, and then prints out a histogram, and the mean and mode of the of the MBTI types.
 */

#define MAXSTRLEN 127
#define MAXSTATLEN 127
#define MBTINUM 16

/* calculates the mode by finding at the index of the array with the highest value
pre-conditions: the array stat of floats
post-conditions: returns the index of the array with the highest value
 */
int mode(float stat[]) {
  float max = 0.0;
  int modeindex = 0;
  for (int line = 0; line < MBTINUM; line++) {
    if (stat[line] > max){
      max = stat[line];
      modeindex = line;
    }
  }
  return modeindex;
}


/* serves as a helper function to find the absolute value of two numbers; if the argument is negative, it changes it to a positive number
pre-conditions: the number must be in float form
post-conditions: returns the number in positive form
 */
float absoluteValue(float num) {
  // if num is negative, changes to positive
  if (num < 0) { 
    num = (-1) * num;
  }
  return num;
}

/* calculates the mean value
pre-conditions: float array
post-conditions: the calculated mean as a float
 */
float calcmean(float stat[]) {
  float sum = 0.0;
  float mean;
  // caculates the cumulative sum of the values in the array
  for (int line = 0; line < MBTINUM; line++) {
    sum +=  stat[line];
  }
  // calculates the mean
  mean = sum / MBTINUM;
  return mean;
}

/* calculates the mean value, and finds the index in the array with the value that is closest to the mean
pre-conditions: the array stat of floats
post-conditions: prints the value of the mean, and returns the index in the arraay with the value closest to the mean
 */
int meanindex(float stat[]) {
  float mean;
  int meanindex = 0;
  // calls the calcmean function
  mean = calcmean(stat);

  // checks if the mean value is in the array; if it is, returns the index
  for (int line = 0; line < MBTINUM; line++) {
    if (stat[line] == mean) {
      meanindex = line;
      return meanindex;
    }
  }
  
  float closest = 100;
  int closestmeanindex = 0;

  // checks for the index of the value that is closest to the mean
  for (int i = 1; i < MBTINUM-1; i++) {
    if (absoluteValue(mean - stat[i]) <= absoluteValue(mean - stat[i-1])) {
      closest = stat[i];
      if (absoluteValue(mean - stat[i]) <= absoluteValue(mean - closest)) {
          closest = stat[i];
          // printf("closest value: %.2f\n", closest);
          closestmeanindex = i;
          // printf("closestmeanindex: %d\n", closestmeanindex);
          return closestmeanindex;
      }
    }
  }
  return closestmeanindex;
}



/*
 reads each line of the file and saves it into the array. performs analysis of the data and outputs a histogram and analysis onto a separate file
preconditions: the file pointer of stream, given that the file is correctly opened in the main function
post-conditions: prints onto file the mbti list, along with the statistics and the histogram, however returns void
 */

int data(FILE * stream) {

  // declarations of values and arrays
  char * mbtilist[MBTINUM+1] = {"INFJ", "INFP", "ENFP", "ENFJ", "INTJ", "INTP", "ENTP", "ENTJ", "ISTJ", "ISTP", "ESTP", "ESTJ", "ISFJ", "ISFP", "ESFP", "ESFJ"};
  float stat[MAXSTATLEN];
  char * newline = NULL;
  char mbti[MAXSTRLEN];
  // file pointer to write in the file
  FILE * outputfile = fopen("output-file", "w");
  int line = 0;

  // reads in each line in the input file
  while (fgets(mbti, MAXSTRLEN, stream) != NULL) {

    // prints the MBTI types
    printf("%-8s", mbtilist[line]);
    fprintf(outputfile, "%-8s", mbtilist[line]);

    // checks for correct input values, else returns error
    if (atof(mbti) != 0) {
      // converts string to float
    stat[line] = atof(mbti);
    printf("%.1f\n", stat[line]);

    // prints out histogram according to stats
    for (int i = 0; i < (stat[line]*2); i++) {
      fprintf(outputfile, "|");
    }

    // prints the percentages
    fprintf(outputfile, "  (%.1f)\n", stat[line]);
    line++;
  }
    else {
      fprintf(outputfile, "Error; input is not the correct data type.\n");
      return 1;
    }
  }

  // prints the statistical analysis
  fprintf(outputfile, "\n\nMode type/ most common MBTI type: %s, with percentage of %.1f%%\n\n", mbtilist[mode(stat)], stat[mode(stat)]);
  fprintf(outputfile, "The mean percentage is %.2f%%.\n", calcmean(stat));
  fprintf(outputfile, "Therefore the MBTI type with the percentage value closest to the mean is %s, with percentage of %.1f%%.\n", mbtilist[meanindex(stat)], stat[meanindex(stat)]);

  return 0;
}



int main (int argc, char * argv[]) {

  printf("This program opens a file containing statistics for the MBTI population in the USA. It then reads in each line, which is a float corresponding to each MBTI type. There is a statistical analysis that is done, and then prints out a histogram, and the mean and mode of the of the MBTI types.\n");

  // check if file can be opened
  if (argc != 2) {
    fprintf (stderr, "Usage: %s filename\n",argv[0]);
    return EXIT_FAILURE;
  }

  // open file and save into a file pointer
  FILE* stream = fopen (argv[1],"r");   

  // verify if opening the file was successful
  if (stream == NULL) {                                       
    fprintf (stderr, "%s: Cannot open %s: %s\n",
             argv[0], argv[1], strerror(errno));
    return EXIT_FAILURE;
  }

  // call the function outputting, which contains what the program does to input, output the data, as well as run the functions that analyze the data
  if (data(stream) == 1){
    printf("\n\nThe program did not run successfully.\n\n"); 
    return EXIT_FAILURE;
  }
  printf("Please check the file named output-file in the same directory; it will contain the histogram and the statistical analysis of the data.\n");

  // close file stream
  if (fclose (stream)) {
    fprintf (stderr, "%s: Error closing file %s: %s\n",
             argv[0], argv[1], strerror(errno));
    return EXIT_FAILURE;
    }
  
  return EXIT_SUCCESS;
}
