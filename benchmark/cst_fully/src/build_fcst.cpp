#include <sdsl/suffix_trees.hpp>
#include <fstream>
#include <string>

using namespace sdsl;

typedef FCST_TYPE fcst_type;
typedef CST_TYPE cst_type;

int main(int argc, char **argv) {
    if(argc < 5) {
        std::cout << "Usage: " << argv[0] << " [input_file] [tmp_dir] [fcst_file] [cst_file]" << endl;
        return 1;
    }

    const char* input_file = argv[1];
    const char* tmp_dir = argv[2];
    const char* fcst_file = argv[3];
    const char* cst_file = argv[4];

    cst_type cst;
    fcst_type fcst;

    {
        cache_config config(true, tmp_dir);
        construct(cst, input_file, config, 1);
        construct(fcst, input_file, config, 1);
    }

    store_to_file(cst, cst_file);
    store_to_file(fcst, fcst_file);

    return 0;
}
