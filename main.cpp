#include <chrono>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int count_quotes_in_file(std::string filename)
{
    std::ifstream file("dat/" + filename);
    std::string line;
    int total_quotes = -1;

    while (std::getline(file, line))
    {
        if (line[0] == '%')
        {
            ++total_quotes;
        }
    }

    if (total_quotes == 0)
    {
        exit(1);
    }

    return total_quotes;
}

std::string get_random_quote_from_filename(std::string filename)
{
    int quotes_counted = 0;
    int quote_index = rand() % count_quotes_in_file(filename);
    bool quote_read = false;
    std::ifstream file("dat/" + filename);
    std::string line;
    std::string quote = "";

    while (std::getline(file, line) && !quote_read)
    {
        if (line[0] == '%')
        {
            if (quotes_counted == quote_index)
            {
                /* read lines until entire quote is consumed */
                while(std::getline(file, line) && !quote_read)
                {
                    if (line[0] == '%')
                    {
                        quote_read = true;
                    }
                    else
                    {
                        if (line[0] != '#')
                        {
                            quote += line + "\n";
                        }
                    }
                }
            }
            else
            {
                ++quotes_counted;
            }
        }
    }

    return quote;
}

std::vector<std::string> fetch_files_from_group(std::string group)
{
    std::ifstream group_file("dat/groups/" + group);
    std::string line;
    std::vector<std::string> filenames;

    while (std::getline(group_file, line))
    {
        if (line[0] != '#')
        {
            filenames.push_back(line);
        }
    }

    return filenames;
}

int get_random_number(int min, int max)
{
    std::default_random_engine generator;
    std::chrono::milliseconds ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch());
    generator.seed(ms.count());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

int main(int argc, char * argv[])
{
    std::string group = "all"; /* default to fetching all files */
    std::vector<std::string> filenames = fetch_files_from_group(group);
    std::string filename = filenames[
        get_random_number(0, filenames.size() - 1)];
    printf("%s\n", get_random_quote_from_filename(filename).c_str());

    return 0;
}
