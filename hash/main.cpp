#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cryptopp/cryptlib.h>
#include <cryptopp/md5.h>

using namespace CryptoPP;
using namespace std;

string calculatehash(const string& input)
{
    Weak::MD5 hash;
    byte digest[Weak::MD5::DIGESTSIZE];
    hash.Update(reinterpret_cast<const byte*>(input.data()), input.size());
    hash.Final(digest);
    stringstream ss;
    for (size_t i = 0; i < Weak::MD5::DIGESTSIZE; i++) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(digest[i]);
    }
    return ss.str();
}

int main()
{
    string filename = "misp4.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "unable to open file " << filename << endl;
        return 1;
    }
    string line;
    while (getline(file, line)) {
        string hash = calculatehash(line);
        cout << "hash of '" << line << "': " << hash << endl;
    }
    file.close();
    return 0;
}