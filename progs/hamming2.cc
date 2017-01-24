// Michael LoPiccolo
// This program is still a draft.

char usage[] =
"Check the Hamming distance of a provided permutation file.\n"
"\n"
"Compile with g++ hamming.cc -std=c++0x -O3 -o hamming\n"
"The -O3 is very important to optimize for improved speed!\n"

"Usage:\n"
"hamming <# symbols> <input filename> [-d <minimum HD> | -a <maximum #agreements>]\n"
"For example, `hamming 6 inperms.txt -a 2` will check 6-length permutations\n"
"to ensure none have more than 2 agreements. Alternately, we could use -d 4.\n"
"You can omit the <# symbols> and we'll try to infer: `hamming PGL32_30.txt` will infer n=32 and d=30.";

#include <iostream>
#include <getopt.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <sstream>

using namespace std;

// Typedef to make everything make a little more sense, just syntax sugar
typedef int* perm;

// uncomment for more information in GDB
// template class std::vector<perm>;

void check(const vector<perm>& perms, int allowed_colisions, int start_index); 

static int n = -1;
static int worst = 0;
static int give_up;
static bool decision;

// Pre-caching space so that we don't have to keep putting stuff on the heap and destroying it.
// We know we'll only make calls to a depth of n.
static vector<perm> **buckets;

int main(int argc, char** argv) {
  int a = -1;
  int d = -1;
  int c;
  while((c = getopt(argc, argv, "d:")) != -1) {
    switch(c) {
      case 'd':
        d = stoi(optarg);
        break;
      case ':':
      case '?':
        cout << usage << endl;
        cout << "Unknown option." << endl;
        exit(2);
    }
  }


  if(argc - optind == 0 || argc - optind > 2) {
    cout << usage << endl;
    exit(1);
  }

  if(argc - optind == 2) {
    n = stoi(argv[optind]);
    if(n <= 0) {
      cout << "Number of symbols should be positive." <<  endl<< usage << endl;
      exit(9);
    }
    optind++;
  }

  ifstream infile(argv[optind]);
  if(!infile) {
    cout << "Error opening file (not found?)." << endl;
    exit(3);
  }

  string line;
  getline(infile, line);
  stringstream ss(line);
  n = 0;
  string junk;
  while(ss >> junk) {
    n++;
  }
  infile.close();
  infile.open(argv[optind]);
  cout << "Inferred number of symbols: " << n << endl;
  if(d != -1) {
    cout << "Checking for Hamming distance: " << d << endl;
  }


  decision = true;
  if(a != -1 && d != -1) {
    cout << usage << endl;
    exit(6);
  }
  else if (a != -1) {
    give_up = a;
  }
  else if (d != -1) {
    give_up = n - d;
  }
  else {
    decision = false;
  }


  buckets = new vector<perm>*[n];
  for(int i = 0; i < n; i++) {
    buckets[i] = new vector<perm>[n];
  }

  map<string, int> renames; // We want to rename all symbols to 0..n-1
  int at = 0;

  vector<perm> perms; //perms[i][n] is the nth position in the permutation i
  string swap;
  int count = 0;

  infile >> swap;
  while(infile.good()) {
    perm temp = new int[n];
    for(int i = 0; i < n; i++) {
      if(!renames.count(swap)) {
        if(at == n) {
          cout << "Input problem, too many symbols found" << endl;
          exit(5);
        }

        renames.emplace(swap, at++);
      }
      temp[i] = renames[swap];
      if (!(infile >> swap)) {
        if(i != n-1) {
          cout << "Input mismatch, not enough symbols in the last permutation" << endl;
          exit(4);
        }
      }
    }
    count++;
    perms.emplace_back(temp);
  }

  check(perms, 0, 0);
  if(decision) {
    cout << "VERIFIED:" << endl;
  }
  cout << "The file has Hamming distance " << n - worst <<  endl<<
    "i.e., there are at most " << worst << " agreements between any pair." << endl;
  exit(0);
}

void check(const vector<perm>& perms, int collisions, int start_index) {
  if(perms.empty())
    return;
  if(n - start_index + collisions <= worst) {
    return;
  }
  if(start_index == n) {
    worst = collisions;
    if(decision && worst > give_up) {
      cout << "NOT VERIFIED:" <<  endl<<
        "Gave up as the Hamming distance is at most " << n - worst <<  endl<<
        "i.e., there is a pair with at least " << worst << " agreements." << endl;
      exit(0);
    }
    return;
  }

  int counts [n];
  for(int i = 0; i < n; i++) {
    counts[i] = 0;
  }
  for(perm a : perms) {
    int ind = a[start_index];
    if(counts[ind] == 0) {
      buckets[start_index][ind].clear();
    }
    buckets[start_index][ind].emplace_back(a);
    counts[ind]++;
  }

  for(int i = 0; i < n; ++i) {
    if(counts[i] > 1) {
      // TODO we could sort by bucket size, to possibly prune better or fail faster.
      check(buckets[start_index][i], collisions + 1, start_index + 1);
    }
  }

  check(perms, collisions, start_index + 1);
}

