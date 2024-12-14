#include <iostream>
#include <fstream>
#include <string>

std::string trim(const std::string& str){
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    if(start == std::string::npos || end == std::string::npos){
        return "";
    }
    return str.substr(start, end - start + 1);
}

void splitAndTrim(const std::string&type, std::string& className, std::string& fields){
    size_t pos = type.find(':');

    if(pos != std::string::npos){
        className = trim(type.substr(0, pos));
        fields = trim(type.substr(pos + 1));
    }
    else{
        className = trim(type);
        fields = "";
    }
}

static void defineType(std::ofstream& writer, std::string baseName, std::string className, std::string fields){

}

static void defineAst(std::string outputDir, std::string baseName, std::vector<std::string> types){
    std::string path = outputDir + "/" + baseName + ".cpp";
    std::ofstream writer(path, std::ios::out | std::ios::binary);

    writer << "#include <iostream>" << std::endl;
    writer << "class " + baseName + " {" << std::endl;
    for (std::string type : types){
        std::string className;
        std::string fields;
        splitAndTrim(type, className, fields);
        defineType(writer, baseName, className, fields);
    }
    writer << "}" << std::endl;
    writer.close(); 
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage generate AST <output directory>" << std::endl;
        exit(64);
    }
    
    std::string output_dir = argv[1];
    std::vector<std::string> expressions = {
        "Binary   : Expr left, Token operator, Expr right",
        "Grouping : Expr expression",
        "Literal  : Object value",
        "Unary    : Token operator, Expr right"
    };

    defineAst(output_dir, "Expr", expressions);    
    return 0;
}