#include <memory>
#include <string>
#include <iostream>
#include "db/DBImpl.hpp"

class Parser {
public:
    Parser(const microkv::Options &options) : options(options), db(std::make_shared<microkv::DBImpl>(options)) {

    }

    auto Parse(const std::string &cmd) -> std::string {
        index = 0;
        this->cmd = cmd;
        SkipWhiteSpaces();
        auto type = ParseOP();

        if (type == EXIT) {
            std::terminate();
        } if (type == GET) {
            const std::string key = ParseString();
            std::string value;
            auto status = db->Get(options, key, value);
            
            if (status == microkv::DBStatus::SUCCESS) {
                return value;
            } else {
                return status.msg;
            }
        } else if (type == SET) {
            const std::string key = ParseString();
            std::string value = ParseString();
            auto &&status = db->Put(options, key, value);
            if (status == microkv::DBStatus::SUCCESS) {
                return "set success";
            } else {
                return "set fail";
            }
        } else if (type == DELETE) {
            const std::string key = ParseString();
            auto &&status = db->Delete(options, key);
            if (status == microkv::DBStatus::SUCCESS) {
                return "delete success";
            } else {
                return "delete fail";
            }
        } else {

        }

        return {};
    }

private:
    enum OP_TYPE {
        GET = 0,
        SET,
        DELETE,
        EXIT,
        UNKNOWN
    };
    void SkipWhiteSpaces() {
        size_t n = cmd.size();
        while (index < n && cmd[index] == ' ') ++index;
    }

    auto ParseOP() -> OP_TYPE {
        size_t start = index;
        SkipWhiteSpaces();
        int n = cmd.length();
        while (index < n && cmd[index] != ' ') ++index;
        std::string op = cmd.substr(start, index - start);
        if (op == "get") {
            return GET;
        } else if (op == "set") {
            return SET;
        } else if (op == "delete") {
            return DELETE;
        } else {
            return UNKNOWN;
        }
    }

    auto ParseString() -> std::string {
        SkipWhiteSpaces();
        int start = index;
        int n = cmd.length();
        while (index < n && cmd[index] != ' ') ++index;
        return cmd.substr(start, index - start);
    }
private:
    size_t index = 0;
    std::string cmd;
    std::shared_ptr<microkv::DBImpl> db;
    microkv::Options options;
};

void PrintHint() {
    std::cout << "> ";
}

void PrintResult(const std::string &result) {
    std::cout << result << std::endl;
}

int main(int argc, char const *argv[]) {
    microkv::Options options;
    Parser parser(options);

    std::string cmd;

    while (true) {
        PrintHint();
        cmd.clear();
        getline(std::cin, cmd);
        std::string &&result = parser.Parse(cmd);
        PrintResult(result);
    }
    return 0;
}