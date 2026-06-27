//Raghad Shatnawi
//Shatha Balaw
//Mais Al-Qudah

#include <iostream>
#include <fstream>
#include <thread>
#include <algorithm>
#include<string>
#include<mutex>
#include<math.h>
#include <vector>
using namespace std;
mutex mtx0,mtx1;
//Global Variables:
long long numOfPrimes = 0;                  // total number of prime numbers 
long long numOfPalindroms = 0;              // total number of palindrome numbers 
long long numOfPalindromicPrime = 0;        // numbers that are BOTH prime and palindrome
long long TotalNums = 0;                    // all the processed numbers in the range
vector<int> PrimeList;             // Stores prime numbers
vector<int> PalindromeList;        // Stores palindrome numbers
vector<int> PalindromicPrimesList; // Stores palindromic prime numbers

struct ThreadArgs
{
    long long start;
    long long end;
    long long ID;
};


// Function to check if a number is palindromic
bool isPalindrome(int num)
{
    string str = to_string(num);// Convert the number to a string
    string reversed_str = string(str.rbegin(), str.rend()); // Check if the string is equal to its reverse
    return str == reversed_str;
}

bool isPrime(int num)
{
    if (num <= 1) {return false;}
    for (int i = 2; i <= std::sqrt(num); ++i){if (num % i == 0) {return false;}}
    return true;
}
long long current=0;
// Function executed by each thread
void processRange(ThreadArgs args) {
     while (args.ID != current) { this_thread::yield();}
   {
         lock_guard<mutex> lock(mtx0);
        cout << "ThreadID=" << args.ID << ", startNum=" << args.start << ", endNum=" << args.end << endl;
        current++;
    }
	for (int num = args.start+1; num <= args.end; ++num) 
	{
	    lock_guard<mutex> lock(mtx1);
		bool prime = isPrime(num);
		bool palindrome = isPalindrome(num);
		if (prime) {PrimeList.push_back(num);numOfPrimes++;}
		if (palindrome) {PalindromeList.push_back(num);numOfPalindroms++;}
		if (prime && palindrome) {PalindromicPrimesList.push_back(num);numOfPalindromicPrime++;}
		TotalNums++;
	}
}

void notUsed(long long ID)
{
while (ID != current) {this_thread::yield();}
cout<<"I am thread: "<<ID<<" and I do nothing"<<endl;
current++;
}

int main(int argc, char** argv)
{   //(T will be passed to the program with the Linux command line)     
    long long T = stoll(argv[1]);  // stoi: function that converts a string (which is what argv[1] is) to ,long long
    if (T <= 0)
    {cerr << "Error: The number of threads cannot be zero." << endl; return 1; }
    //reading the file
    fstream input;
    input.open("in.txt", ios::in);
    long long startRange, endRange;
    input >> startRange >> endRange;
    input.close();
    thread* threads = new thread[T];
    ThreadArgs* threadArgs = new ThreadArgs[T];
    int threadStart = startRange;
    // Number of numbers to be processed by each thread
    long long rangeSize = endRange - startRange;
    long long numbersPerThread = rangeSize / T;
    long long remaining = rangeSize % T;
    long long TID = 0;// for thread ID
  // Create threads to process the range
    for (int i = 0; i < T; ++i)
    { // Divide the range among threads
        long long threadEnd = threadStart + numbersPerThread;
        if (i + 1 > rangeSize) { threads[i] = thread(&notUsed, TID++); continue; }
        if (i < remaining) { threadEnd++; }  // Distribute the remaining numbers
        threadArgs[i] = { threadStart, threadEnd, TID }; // 
        threads[i] = thread(processRange, threadArgs[i]);
        threadStart = threadEnd;  // Update start for next thread
        if (threadEnd > endRange) { cout << "error" << endl; }
        TID++;
    }
    for (int i = 0; i < T; i++){threads[i].join();}
    // Output results
    cout << "Total numbers processed: " << TotalNums << endl;
    cout << "Total prime numbers: " << numOfPrimes << endl;
    cout << "Total palindrome numbers: " << numOfPalindroms << endl;
    cout << "Total palindromic prime numbers: " << numOfPalindromicPrime << endl;
    // Open the output file for writing results
    fstream output;
    output.open("output.txt", ios::out);
    output << "The prime numbers are:" << endl<<endl;
    for (const auto&prime : PrimeList) { output << prime << endl<<endl; }
    output << "The palindrome numbers are:" << endl<<endl;
    for (const auto&palindrome : PalindromeList) { output << palindrome << endl<<endl;}
    output << "The palindromicPrime numbers are:" << endl<<endl;
    for (const auto&palindromicPrime : PalindromicPrimesList) {output << palindromicPrime << endl<<endl;}
    // Close the output file
    output.close();
    delete[] threadArgs;
    delete[] threads;
    return 0;
}
