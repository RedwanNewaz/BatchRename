//
// Created by robor on 8/8/2020.
//

#ifndef RENAME_REFACTORFILENAME_H
#define RENAME_REFACTORFILENAME_H
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <map>
#include <list>
#include <cassert>
#include <regex>
#include <iterator>
#include <algorithm>
#include <random>
#include <variant>
#define SAMPLE_SIZE 5
using namespace std;
namespace fs = boost::filesystem;

template <template <typename, typename> class Container,
        typename Value,
        typename Allocator=std::allocator<Value> >
inline void print_list(const Container<Value, Allocator> & data)
{
    for( auto& d: data)
        cout<<d<<"\t";
    cout<<endl;
}

class RefactorFilename {
    using NAMEMAP = map<string, list<string>>;
public:
    RefactorFilename(const string& dir_path);
    void sample_names();
    void zero_padding(const char* pad, int part_index);
    void remove_parts(int remove_id);
    void set_prefix(const string& prefix);
    void write_files();

private:
    const string dir_path;
    NAMEMAP data_map;
    std::size_t max_parts_len, avg_len;
    string prefix;

protected:
    void mf_read_files();
    void mf_sep_extention(list<string>& item);
    void mf_sep_filename(list<string>& item);

};


#endif //RENAME_REFACTORFILENAME_H
