#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

#include "paddle2onnx/converter.h"

int main(int argc, char* argv[]) {

    std::string model_dirname;
    std::string model_filename, params_filename;
    std::string save_file;
    int opset_version = 11;
    bool
            auto_upgrade_opset = true,
            verbose = true,
            enable_onnx_checker = true,
            enable_experimental_op = true,
            enable_optimize = true;
    using CustomOpInfo = std::map<std::string, std::string>;
    const CustomOpInfo& info = CustomOpInfo();
    std::string deploy_backend = "onnxruntime";
    std::string calibration_file = "";
    std::string external_file = "";
    bool export_fp16_model = false;

    try {
        using boost::program_options::value;
        using boost::program_options::options_description;
        using boost::program_options::parse_command_line;
        using boost::program_options::variables_map;

        options_description desc{"Paddle2ONNX CLI v0.0.1"};
        desc.add_options()
                ("help,h", "Print help message.")
                ("model-dir,d",value<std::string>(&model_dirname)->required())
                ("model-filename,m",value<std::string>(&model_filename)->default_value("inference.pdmodel"))
                ("params-filename,p", value<std::string>(&params_filename)->default_value("inference.pdiparams"))
                ("save-file,o", value<std::string>()->required())
                ("external-filename,t", value<std::string>(&external_file)->default_value("external_data"))
                ("opset-version,n", value<int>(&opset_version)->default_value(9))
                ;

        variables_map vm;

        boost::program_options::store(parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        model_dirname = vm["model-dir"].as<std::string>();
        save_file = vm["save-file"].as<std::string>();
    }
    catch (const boost::program_options::error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    char* out = nullptr;
    int size = 0;
    char* calibration_cache = nullptr;
    int cache_size = 0;
    bool save_external;

    model_filename = model_dirname + "/" + model_filename;
    params_filename = model_dirname + "/" + params_filename;

    if (!paddle2onnx::Export(model_filename.c_str(), params_filename.c_str(), &out, &size,
                             opset_version, auto_upgrade_opset, verbose,
                             enable_onnx_checker, enable_experimental_op, enable_optimize,
                             nullptr, 0, deploy_backend.c_str()/*, &calibration_cache,
                             &cache_size, external_file.c_str(), &save_external,
                             export_fp16_model*/)) {
        std::cerr << "Paddle model convert failed." << std::endl;
        return 1;
    }

    std::ofstream ofs(save_file, std::ios::out | std::ios::binary);

    if (ofs.is_open()) {
        ofs.write(out, size);
    }
}
