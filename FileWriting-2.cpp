//////////////////////////////////////////////////////////////
//Sailesh Bechar
//Lab 7  
//Question 3

//////////////////////////////////////////////////////////////
//
// local #includes and function declarations; do not remove
//
#include <fstream>
#include <math.h>
#include <iostream>
using namespace std;

#ifndef MARMOSET_TESTING
#include <iostream>
#endif

//////////////////////////////////////////////////////////////
//
// struct definitions 
//
#ifndef MARMOSET_TESTING
struct Student{
	int studentID;
	int grade;
};

struct Dataset{
	int numStudents;
	Student* students;
};

struct Rejects{
	int numRejects;
	int* rejects;
};

struct Mode{
	int numModes;
	int* modes;
};

struct Statistics{
	int   minimum;
	float average;
	int   maximum;
	float popStdDev;
	float smplStdDev;
	Mode  mode;
	int   histogram[10];
};
#endif

//////////////////////////////////////////////////////////////
//
// Function declarations; do not remove
//
int readCSV(const char filename[],const int minAcceptableID, 
				const int maxAcceptableID,Dataset& data, 
				Rejects& rejects);

int computeStatistics(Dataset& data, Statistics& stats);

int writeCSV(const char filename[], const Statistics& stats);

//////////////////////////////////////////////////////////////
//
// Function definitions. PLEASE USE HELPER FUNCTIONS
//
int readCSV(const char filename[],const int minAcceptableID, 
				const int maxAcceptableID,Dataset& data, 
				Rejects& rejects){
	 const int maxLineLength = 100;
  char line[maxLineLength];
  char newfilename[100];
  int zop = 0;
  int period = 0;
  data.numStudents = 0;
  
  if (maxAcceptableID <= minAcceptableID){
    return -1;
  }

  //Check if csv file
  while (filename[zop] != 0){
  	newfilename[zop] = filename[zop];
  	if (filename[zop] == 46){
  		period = zop;
  	}
  	zop++;
  }

  if (period == 0){
  	newfilename[zop] = '.';
  	newfilename[zop+1] = 'c';
  	newfilename[zop+2] = 's';
  	newfilename[zop+3] = 'v';
  	newfilename[zop+4] = 0 ;
  }
  else{
  	newfilename[period] = '.';
  	newfilename[period+1] = 'c';
  	newfilename[period+2] = 's';
  	newfilename[period+3] = 'v';
  	newfilename[period+4] = 0;	
  }


  ifstream infile;
  infile.open(newfilename);
  if (!infile.is_open()){
    return -1;
  }
  
  while (!infile.eof()){
    infile.getline(line, maxLineLength);
    if (line[0] != 0){
      data.numStudents++;
    }
  }
  data.students = new Student[data.numStudents];
  rejects.rejects = new int[data.numStudents];

  int rejArray[data.numStudents];

  int i = 0;
  char j;
  int z = 0;
  int gradesize = 0;
  int linecount = 0;
  int rejectcounter = 0;
  int counter = 0;
  int gcount = 0;
  int scount = 0;
  int alphaindex = 0;
  int numcounter = 0;
  int commcounter = 0;
  int tempgrades[data.numStudents];
  int newcounter = 0;
  bool rejectstate = false;
  infile.clear();
  infile.seekg(0, ios::beg);

  while (linecount < data.numStudents){
    i = 0;
    counter = 0;
    rejectstate = false;
    gcount = 0;
    scount = 0;
    data.students[linecount].grade = 0;
    tempgrades[linecount] = 0;
    data.students[linecount].studentID = 0;
    gradesize = 0;
    alphaindex = 0;
    commcounter = 0;

      if (line[i]!= 0){
      infile.getline(line, maxLineLength);
      

      while (line[alphaindex] != 0){
        if (line[alphaindex] > 57 || line[alphaindex] < 48 && line[alphaindex] != ' ' && line[alphaindex] != '-' && line[alphaindex] != ',' && (line[alphaindex] != 13 && line[alphaindex] != 9)){
          rejectstate = true;
        }
        if (line[alphaindex] == ','){
        	++commcounter;
        }
        alphaindex++;
      }
      if (commcounter != 1){
        rejectstate = true;
	  }
      while (line[i] == ' ' || line[i] == '\t'){
        i++;
      }

      while (line[i+counter] >= 48 && line[i+counter] <= 57){
        counter++;
      }

      while (line[i] >= 48 && line[i] <= 57){
        z = line[i] - 48;
        data.students[linecount].studentID += z * pow(10, counter-scount-1);
        scount++;
        i++;
      }
      if (data.students[linecount].studentID < minAcceptableID || data.students[linecount].studentID > maxAcceptableID){
        rejectstate = true;
      }
      while (line[i] == ' ' || line[i] == '\t'){
        i++;
      }
      if (line[i] > 33 && line[i] != 44){
      	rejectstate = true;
      }
      if (line[i] == ','){
        i++;
      }
      while (line[i] == ' ' || line[i] == '\t'){
        i++;
      }
      if (line[i] == '-'){
        rejectstate = true;
      } 
      else{
        while (line[i + gradesize] >= 48 && line[i + gradesize] <= 57){
          gradesize++;
        }
        while (line[i] >= 48 && line[i] <= 57){
          z = line[i] - 48;
          tempgrades[linecount] += z * pow(10, gradesize-gcount-1);,
          gcount++;
          i++;
        }
        if (tempgrades[linecount] > 100 ){

          rejectstate = true;
        }
        while (line[i] == ' ' || line[i] == '\t'){
          i++;
        }
        if (line[i] > 33 && line[i] != 44){
      	rejectstate = true;
      	}   
      }

      if (gcount == 0 || scount == 0){
      	rejectstate = true;
      }
      if (rejectstate == true){
      	rejects.rejects[rejectcounter] = linecount +1;

        rejectcounter++;
      }
      else{
      	data.students[newcounter].grade = tempgrades[linecount];
      	newcounter++;
      }

    }
    linecount++;
  }
  infile.close();



  if (rejectcounter > 10){
     return -1;
  }
  data.numStudents = newcounter;

  rejects.numRejects = rejectcounter;

  return rejectcounter;


}

int computeStatistics(Dataset& data, Statistics& stats){
	//Calculate Minimum
	stats.minimum = data.students[0].grade;
	for (int x = 0; x < data.numStudents; x++){
	    if(stats.minimum > data.students[x].grade){
	      stats.minimum = data.students[x].grade;
	    }
  	}
  	//Calculate Maximum
  	stats.maximum = data.students[0].grade;
  	for (int x = 0; x < data.numStudents; x++){
	    if(stats.maximum < data.students[x].grade){
	      stats.maximum  = data.students[x].grade;
	    }
 	}
 	//Calculate Average
 	float sum = 0;
	for (int x = 0; x < data.numStudents; x++){
	    sum += data.students[x].grade;
	}

  	stats.average = sum/ data.numStudents;

  	//Calculate Population Standard Deviation
	float avgsum = 0;
	for (int x = 0; x < data.numStudents; x++){
		avgsum += pow((data.students[x].grade - stats.average), 2);
	}

	stats.popStdDev = sqrt(pow(data.numStudents, -1) * avgsum);

	//Calculate Sample Standard Deviation
	stats.smplStdDev = sqrt(pow(data.numStudents - 1, -1) * avgsum);

	//Calculate Mode
	int counter[data.numStudents];
	int i = 0;
	int repeatcounter = 0;
	int y = 0;
	int array[data.numStudents];
	int modecounter[data.numStudents];
	int temp;
	int modetemp;
	int temparray;

	stats.mode.modes = new int[data.numStudents];

	for (int x = 0; x < data.numStudents; x++){
		array[x]= data.students[x].grade;
	}
	for(int z=1;z<data.numStudents;++z)
	{
	  for(int j=0;j<(data.numStudents-z);++j)
	      if(array[j]>array[j+1])
	      {
	          temparray =array[j];
	          array[j]=array[j+1];
	          array[j+1]=temparray;
	      }
	}

	while( i < data.numStudents){
		y = 0;

		while (array[i] == array[i+y]){
		  counter[repeatcounter] = y++;
		  stats.mode.modes[repeatcounter] = array[i];
		}
		i=i+y;
		repeatcounter++;
	}

	for(int z=1;z<repeatcounter;++z)
	{
	  for(int j=0;j<(repeatcounter-z);++j)
	      if(counter[j]<counter[j+1])
	      {
	          temp=counter[j];
	          counter[j]=counter[j+1];
	          counter[j+1]=temp;

	          modetemp=stats.mode.modes[j];
	          stats.mode.modes[j]=stats.mode.modes[j+1];
	          stats.mode.modes[j+1]=modetemp;
	      }
	}
	
	stats.mode.numModes = 0;
	for (int x = 0; x < repeatcounter; x++){
		if (counter[0] == counter[x]){
		  stats.mode.numModes++;
		}
	}

	//Calculate Histograms
	if (!stats.histogram){
		return -1;
	}
	for (int x = 0; x <= 10; x++){
		stats.histogram[x] = 0;
	}

	for (int x = 0; x < data.numStudents; x++){
		if (data.students[x].grade >= 0 && data.students[x].grade < 10){
	       stats.histogram[0]++;
	    }
	    if (data.students[x].grade >= 10 && data.students[x].grade < 20){
	      stats.histogram[1]++;
	    }
	    if (data.students[x].grade >= 20 && data.students[x].grade < 30){
	      stats.histogram[2]++;
	    }
	    if (data.students[x].grade >= 30 && data.students[x].grade < 40){
	      stats.histogram[3]++;
	    }
	    if (data.students[x].grade >= 40 && data.students[x].grade < 50){
	      stats.histogram[4]++;
	    }
	    if (data.students[x].grade >= 50 && data.students[x].grade < 60){
	      stats.histogram[5]++;
	    }
	    if (data.students[x].grade >= 60 && data.students[x].grade < 70){
	      stats.histogram[6]++;
	    }
	    if (data.students[x].grade >= 70 && data.students[x].grade < 80){
	      stats.histogram[7]++;
	    }
	    if (data.students[x].grade >= 80 && data.students[x].grade < 90){
	      stats.histogram[8]++;
	    }
	    if (data.students[x].grade >= 90 && data.students[x].grade <= 100){
	      stats.histogram[9]++;
	    }
	}



	return 0;
}

int writeCSV(const char filename[], const Statistics& stats){

	int top = 0;
	int period;
	char newfilename[100];

	while (filename[top] != 0 && filename[top] != 46){
		newfilename[top] = filename[top];
  		top++;
  	}
  	
  	newfilename[top] = '.';
  	newfilename[top+1] = 's';
  	newfilename[top+2] = 't';
  	newfilename[top+3] = 'a';
	newfilename[top+4] = 't';
	newfilename[top+5] = 0;


	ofstream outfile;
	outfile.open(newfilename);
	if (!outfile.is_open()){
		return -1;
	}
	
	outfile << "Minimum: " << stats.minimum << "\r\n";
	outfile << "Average: " << stats.average << "\r\n";
	outfile << "Maximum: " << stats.maximum << "\r\n";
	outfile << "Population Standard Deviation: " << stats.popStdDev << "\r\n";
	outfile << "Sample Standard Deviation: " << stats.smplStdDev << "\r\n";
	outfile << "Modes: ";
	for (unsigned char i = 0; i < stats.mode.numModes; i++)
	{
		outfile << stats.mode.modes[i];
		if (i < stats.mode.numModes -1){
			outfile << ", "; 
		}
		
	}
	outfile << "\r\n";
	outfile << "Histogram: " << "\r\n";
	for (unsigned char i = 0; i < 10; i++)
	{
		outfile << "[" << (i*10) << "-" << ((((i+1)*10) - 1) + i/9) << "]: " << stats.histogram[i] << "\r\n";
	}


	outfile.close();
	return 0;
}
//////////////////////////////////////////////////////////////
//
// Test Driver
//
// Do not remove the #ifndef and its associated #endif
//

#ifndef MARMOSET_TESTING

#define isNaN(X) (X != X)  // NaN is the only float that is not equal to itself

int main(const int argc, const char* const argv[]) {
  	
	// Some test driver code here ....
	int minAcceptableID = 10000;
	int maxAcceptableID = 2000000;

	Dataset data = {0};
	Rejects rejects = {0};
	Statistics stats = {0};

	

	if(readCSV(argv[1], minAcceptableID, maxAcceptableID, data, rejects) < 0)
	{
		std::cout << ".csv file could not be read" << std::endl;
	}

	if (computeStatistics(data, stats) < 0)
	{
		std::cout << "Stats could not be computed" << std::endl;
	}

	if (writeCSV(argv[1], stats) < 0)
	{
		std::cout << ".stat file could not be written" << std::endl;
	}

	std::cout << "Average: " << stats.average << std::endl;
	std::cout << "Minimum: " << stats.minimum << std::endl;
	std::cout << "Maximum: " << stats.maximum << std::endl;
	std::cout << "Population standard deviation: " << stats.popStdDev << std::endl;
	std::cout << "Sample standard deviation: " << stats.smplStdDev << std::endl;
	
	
	for (unsigned char i = 0; i < stats.mode.numModes; i++)
	{
		std::cout << "Mode: " << stats.mode.modes[i] << std::endl;
	}

	for (unsigned char i = 0; i < 10; i++)
	{
		std::cout << "Histogram bin [" << (i*10) << ", " << ((((i+1)*10) - 1) + i/9) << "]: " << stats.histogram[i] << std::endl;
	}
	

	return 0;
}

#endif
