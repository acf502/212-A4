/*
Name: Aaron Colin Foote
Date: 30th May 2013
User: acf502 / 4258770
File.: server.cc for CSCI212 Assignment 4
*/

#include "aux.h"

int main()
{
	//VARIABLES
	
	//PROGRAM TIME VARS
	clock_t t1, t2;
	t1 = clock();
	
	//FILE PROCESSOR VARIABLES
	char filename[MAXLENGTH];
	string line;
	ifstream fin;
	
	//DATA STORAGE
	map<char, int> letters;
	map<string, int> words; // Create vector to hold our words
	map<string, int> badwords;
	
	//DATA CALCULATION
	int MostLetterCount = 0, lines = 0, charCount = 0, spaceCount = 0, wordCount = 0, longestWordLength = 0, mostWordCount = 0;
	float avgWord = 0, totalWordLength = 0;
	char MostLetter;
	string mostWord = "", longestWord = "";
	
	//MAIN OPERATION

	//open input file
	//REMOVE NEXT LINE:
	cin >> filename;
	fin.open(filename, ios::in);
	if (!fin.good())
	{
		cout << "File did not open successfully" << endl;
		exit(1);
	}
	//while !eof
	while (! fin.eof())
	{
		//read line from file
		getline(fin, line);	
		//preprocess line
		for (int i = 0; i < line.length();i++)	// input.length() gets the length of the file line
		{
			if (isalnum(line[i]) || line[i] == '-' || line[i] == '\'' || isspace(line[i]) || line[i] == '\n')
			{
				if (isalpha(line[i]))
					line[i]=tolower(line[i]);	 // convert every letter character of file line to lowercase
				letters[line[i]]++;
				if (letters[line[i]] > MostLetterCount && isalnum(line[i]))
				{
					MostLetterCount = letters[line[i]];
					MostLetter = line[i];
				}
			}
			if (isspace(line[i]))
				spaceCount++;
			else
				charCount++;
			
		}
		//for each word in line
		string buf; // Have a buffer string
		stringstream ss(line); // Insert the string into a stream

		while (ss >> buf)
		{
			wordCount++;
			for(int i = 0 ; i < buf.size(); i++)   
			{   
				if(ispunct(buf[i]) && buf[i] != '-' && buf[i] != '\'') 
				{
					buf.erase(i,1); 
				}
			}
			totalWordLength += buf.size();
			if (buf.size() > longestWordLength)
			{
				longestWordLength = buf.size();
				longestWord = buf;
			}
			if (words[buf] > mostWordCount)
			{
				mostWordCount = words[buf];
				mostWord = buf;
			}
			char aWord[50];
			bool found = false;
			ifstream dictin("/usr/dict/words");	//open dictionary
			int linemarker = (lines + 1);
			
			for ( ; dictin.good(); linemarker++) 	//for each word in dictionary
			{
				dictin >> aWord;
				if (strcmp(aWord, buf.c_str()) == 0)	//compare words
				{
					words[buf]++;
					found = true;
					break;	//break;
				}					
			}
			dictin.close();		//close dictionary
			if (! found)	//if !found
			{
				badwords[buf] = (lines + 1);
			}
			
		
		}	//end for
		letters['\n']++;	//increase line count and \n count
		lines++;
	}
	
	avgWord = totalWordLength / wordCount;
	cout << "Filename: \t\t\t\t\t" << filename << endl;
	cout << "Characters in file: \t\t\t\t" << (charCount + spaceCount) << endl;
	cout << "Characters in file (without spaces): \t\t" << charCount << endl;
	cout << "Words in file : \t\t\t\t" << wordCount << endl;
	cout << "Lines in file: \t\t\t\t\t" << lines << endl;
	cout << setprecision(2) << fixed;
	cout << "Average word length:\t\t\t\t" << avgWord << endl;
	cout << setprecision(0) << fixed;
	cout << "Largest word:\t\t\t\t\t" << longestWord << endl;
	cout << "Most frequently used word:\t\t\t" << mostWord << endl;
	
	cout << "Most common letter (letters and values only): \t" << MostLetter << endl;
	cout << "Occurences of most letter: \t\t\t" << MostLetterCount << endl;

	cout << endl;
	cout << "20 most common characters in file:" << endl;
	
	int tmpMaxLetter = MostLetterCount;
	for (int limit = 20; limit > 0; tmpMaxLetter--)
	{
		for( map<char, int>::iterator it=letters.begin(); it!=letters.end(); ++it)
		{
			if ((*it).second == tmpMaxLetter)
			{
				cout << (21 - limit) << "\t";
				if ((*it).first == '\n')
					cout << "'\\n'" << ": " << (*it).second << endl;
				else if ((*it).first == '-')
					cout << "'-'" << ": " << (*it).second << endl;
				else if ((*it).first == '\'')
					cout << "'" << ": " << (*it).second << endl;
				else
					cout << "  " << (*it).first << " : " << (*it).second << endl;
				limit--;
			}
		}
	}
	
	cout << "\n\nBad words found in file:\n\n";
	
	cout << "Line No.\tMiss-spelt word";
	
	for( map<string, int>::iterator it=badwords.begin(); it!=badwords.end(); ++it)
	{
		cout << (*it).second << "\t\t" << (*it).first << endl;
	}
	
	cout << "\n\n";
	//close input file
	fin.close();
	//print processing time

	t2 = clock();
	float diff = ((float)t2 - (float)t1);


	cout << "Total processing time = " << time << " sec\n\n";

	return 0;
}
