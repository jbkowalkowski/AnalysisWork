
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
  std::streambuf *strbuf;
  std::streambuf *backup_cout, *backup_cerr;
  std::ostringstream output_cout, output_cerr;
  std::ofstream output_all("output.txt");

  if(!output_all)
    {
      std::cerr << "failure to create output file\n";
      return -1;
    }
  
  backup_cout = std::cout.rdbuf();
  strbuf = output_cout.rdbuf();
  std::cout.rdbuf(strbuf);     
  backup_cerr = output_cerr.rdbuf();
  std::cerr.rdbuf(strbuf);     

  std::cout << "This is written to cout\n";
  std::cerr << "This is written to cerr\n";

  std::cout.rdbuf(backup_cout);
  std::cerr.rdbuf(backup_cout);

  output_all << output_cout.str() << "\n";
  output_all << output_cerr.str() << "\n";
  return 0;
}
