#include <iostream>
#include <boost/program_options.hpp>
#include "RefactorFilename.h"
using namespace boost::program_options;
int main(int argc, char *argv[]) {
    std::cout << "Rename Initialize" << std::endl;
    assert(argc>0 && "No directory is given");

    options_description desc{"Options"};
    desc.add_options()
            ("help,h", "Help screen")
            ("samples", "random samples after modification")
            ("pad",value<std::string>(), "zero padding format e.g., %04d")
            ("prefix",value<std::string>(), "prefix before the filename")
            ("index", value<int>()->default_value(0), "padding index")
            ("remove", value<int>(), "index for the part to be removed");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    RefactorFilename rename(argv[1]);
    if(vm.count("remove"))
        rename.remove_parts(vm["remove"].as<int>());

    if(vm.count("prefix"))
        rename.set_prefix(vm["prefix"].as<string>());

    if(vm.count("pad"))
    {
        assert(vm.count("index"));
        rename.zero_padding(vm["pad"].as<string>().c_str(), vm["index"].as<int>());
    }

    if(vm.count("samples"))
    {
        rename.sample_names();
    }
    else
        rename.write_files();
    return 0;
}
