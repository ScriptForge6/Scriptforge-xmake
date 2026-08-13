#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <cstdint>

std::uint32_t generate_rand() {
    // 线程安全 + 真随机初始化
    static std::mt19937_64 rng{ std::random_device{}() };
    static std::uniform_int_distribution<std::uint32_t> dist{ 0, 0x7FFFFFFF };
    return dist(rng);
}

int main() {
    const auto NAMESPACE_RND = generate_rand();
    const auto CLASS_RND = generate_rand();
    const auto FUNC1_RND = generate_rand();
    const auto FUNC2_RND = generate_rand();
    const auto FUNC3_RND = generate_rand();
    const auto FUNC4_RND = generate_rand();
    const auto FUNC5_RND = generate_rand();
    const auto FUNC6_RND = generate_rand();
    const auto VAR1_RND = generate_rand();
    const auto VAR2_RND = generate_rand();
    const auto VAR3_RND = generate_rand();
    const auto VAR4_RND = generate_rand();

    std::ofstream file(RANDOM_FILE);
    
    if (!file.is_open()) {
        std::cerr << "错误：无法创建文件！\n";
        return 1;
    }

    // 写入自动生成的模块
    file << "// Auto-generated - DO NOT EDIT\n";
    file << "export module Scriptforge.AntiDebug.Random;\n\n";
    file << "#define NS_RANDOM    " << NAMESPACE_RND << '\n';
    file << "#define CLS_RANDOM   " << CLASS_RND << '\n';
    file << "#define F1_RANDOM    " << FUNC1_RND << '\n';
    file << "#define F2_RANDOM    " << FUNC2_RND << '\n';
    file << "#define F3_RANDOM    " << FUNC3_RND << '\n';
    file << "#define F4_RANDOM    " << FUNC4_RND << '\n';
    file << "#define F5_RANDOM    " << FUNC5_RND << '\n';
    file << "#define F6_RANDOM    " << FUNC6_RND << '\n';
    file << "#define V1_RANDOM    " << VAR1_RND << '\n';
    file << "#define V2_RANDOM    " << VAR2_RND << '\n';
    file << "#define V3_RANDOM    " << VAR3_RND << '\n';
    file << "#define V4_RANDOM    " << VAR4_RND << '\n';

    if (!file) {
        std::cerr << "错误：文件写入失败！\n";
        std::cin.get();
        return 1;
    }

    std::cout << "写入成功！\n";

    file.close();
    return 0;
}
