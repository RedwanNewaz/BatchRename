//
// Created by robor on 8/8/2020.
//

#include "RefactorFilename.h"

RefactorFilename::RefactorFilename(const string &dir_path):dir_path(dir_path) {
    prefix = "";
    mf_read_files();
}

void RefactorFilename::mf_read_files() {

    fs::path p (dir_path);

    fs::directory_iterator end_itr;


    int total_files = 0;
    int sum_parts_len = 0;
    max_parts_len = 0;
    // cycle through the directory
    for (fs::directory_iterator itr(p); itr != end_itr; ++itr, ++total_files)
    {
        // If it's not a directory, list it. If you want to list directories too, just remove this check.
        if (is_regular_file(itr->path())) {
            // assign current file name to current_file and echo it out to the console.
            string filename = itr->path().filename().string();
//            cout << filename << endl;
            list<string> item{filename};
            mf_sep_extention(item);
            mf_sep_filename(item);
            sum_parts_len+=item.size();
            data_map[filename]= item;
            max_parts_len = max(item.size(), max_parts_len);
        }
    }
    avg_len = sum_parts_len/total_files;

}

void RefactorFilename::mf_sep_extention(list<string> &item) {
    assert(item.size() == 1);
    list<string> result;
    boost::split(result, item.front(), boost::is_any_of("."));
    item.clear();
    item.push_back(result.front());
    item.push_back(result.back());
}

void RefactorFilename::mf_sep_filename(list<string>& item) {
    auto filename = item.front();
    auto ext = item.back();
    item.clear();
    std::regex rgx("[\\s.\\-,_;]+");
    std::sregex_token_iterator iter(filename.begin(),
                                    filename.end(),
                                    rgx,
                                    -1);
    std::sregex_token_iterator end;
    std::vector<int> result;
    do
    {
        item.push_back(*iter);
    }while(++iter != end);
    item.push_back(ext);
}

void RefactorFilename::set_prefix(const string& prefix)
{
    this->prefix = prefix;
}

void RefactorFilename::zero_padding(const char* pad, int part_index) {

    if (std::regex_match (pad, std::regex("(^\\%)+(\\d+)+[d]$") ))
        std::cout << "string literal matched\n";
    else
    {
        cerr <<"[Error]: padding literal did not match\n";
        return;
    }
    cout<<"[RefactorFilename] zero_padding with "<< pad <<endl;
    //TODO regex check pad and part index
    for(auto pit = data_map.begin(); pit != data_map.end(); ++pit )
    {
        int j = 0;
        list<string>update;
        for(auto qit = pit->second.begin(); qit != pit->second.end(); ++qit,++j)
        {
            if(j == part_index)
            {
                char s[100];
                sprintf(s, pad, atoi(qit->c_str()));
                update.push_back(s);
            } else
            {
                update.push_back(*qit);
            }
        }
        data_map[pit->first] = update;
    }
}

void RefactorFilename::sample_names() {
    vector<int> comparator(max_parts_len, 0);
    int i = 0;
    do{
        auto it = data_map.begin();
        std::advance(it, rand() % data_map.size());
        string random_key = it->first;
        if(it->second.size()>=avg_len)
        {
            static auto ref = it->second;
            int j = 1;
            auto jit = ++it->second.begin();
            for(auto rit = ++ref.begin(); rit != --ref.end(); ++rit,++jit, ++j)
            {
                comparator[j] += (rit->compare(*jit)==0)?1:0;
            }
            cout<<"[Sample]: -> ";
            print_list(it->second);
        }
    }while (++i<=SAMPLE_SIZE);
    cout<<"[RefactorFilename]: Parts Same  ";
    print_list(comparator);
    std::iota(comparator.begin(), comparator.end(),0);
    cout<<"[RefactorFilename]: Parts Index ";
    print_list(comparator);
}

void RefactorFilename::write_files() {

    for(auto it = data_map.begin(); it != data_map.end(); ++it)
    {
        string s = prefix;
        int count = it->second.size()-1;
        for (auto &piece : it->second)
        {
           if(piece.compare(it->second.back())==0)
                s += "."+piece;
            else if(avg_len>2 && --count>0)
                s += piece+"_";
            else
                s += piece;

        }
        fs::path new_path{dir_path}, old_path{dir_path};
        try {
            fs::rename(old_path/it->first, new_path/s);
        }
        catch (std::exception &e) {
            cerr<<e.what()<<endl;
        }

    }
}

void RefactorFilename::remove_parts(int remove_id) {

    for(auto it = data_map.begin(); it != data_map.end(); ++it)
    {
        int N = it->second.size();
        if(N>= avg_len && remove_id<N)
        {
            auto jit = it->second.begin();
            advance(jit, remove_id);
            it->second.erase(jit);
        }
    }

}
