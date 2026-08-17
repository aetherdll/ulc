#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>

// Yardımcı Fonksiyon: Metin sonu kontrolü
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) return false;
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

// Yardımcı Fonksiyon: Tire (-) işaretlerini temizleme
std::string cleanFlag(const std::string& flag) {
    size_t start = flag.find_first_not_of('-');
    return (start == std::string::npos) ? "" : flag.substr(start);
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cout << "Kullanım: ulc -[format] -[arch] <girdi dosyaları> <çıktı dosyası>\n";
        std::cout << "Örnek  : ulc -dll -x86_64 main.cpp helper.c header.h kutuphane.dll\n";
        return 1;
    }

    std::string fmt = cleanFlag(argv[1]);  // dll, so, dlib
    std::string arch = cleanFlag(argv[2]); // x86, x86_64, arm64, arm32
    
    std::string outputFile = argv[argc - 1];
    std::vector<std::string> sourceFiles;
    bool hasCpp = false;

    // Girdi dosyalarını tara (Başlık dosyalarını ele, kaynak dosyaları topla)
    for (int i = 3; i < argc - 1; ++i) {
        std::string file = argv[i];
        
        if (endsWith(file, ".cpp") || endsWith(file, ".cxx") || endsWith(file, ".cc")) {
            sourceFiles.push_back(file);
            hasCpp = true;
        } else if (endsWith(file, ".c")) {
            sourceFiles.push_back(file);
        }
        // .h ve .hpp dosyaları Clang komutuna doğrudan verilmez, bu yüzden atlanıyor.
    }

    if (sourceFiles.empty()) {
        std::cerr << "[HATA] Derlenecek geçerli bir kaynak dosyası (.c, .cpp) bulunamadı.\n";
        return 1;
    }

    // Target (Hedef) Haritası
    std::map<std::pair<std::string, std::string>, std::string> targetMap = {
        {{"dll", "x86"},       "i686-w64-mingw32"},
        {{"dll", "x86_64"},    "x86_64-w64-mingw32"},
        {{"so", "arm64"},      "aarch64-linux-gnu"},
        {{"so", "arm32"},      "arm-linux-gnueabihf"},
        {{"dlib", "arm64"},    "arm64-apple-darwin"},
        {{"dlib", "arm32"},    "armv7-apple-darwin"}
    };

    auto key = std::make_pair(fmt, arch);
    if (targetMap.find(key) == targetMap.end()) {
        std::cerr << "[HATA] Desteklenmeyen kombinasyon: -" << fmt << " -" << arch << "\n";
        return 1;
    }

    std::string targetTriple = targetMap[key];
    std::string compiler = hasCpp ? "clang++" : "clang";

    // Clang Sistem Komutunun Oluşturulması
    std::string systemCmd = compiler + " --target=" + targetTriple + " -shared -fPIC";
    for (const auto& src : sourceFiles) {
        systemCmd += " " + src;
    }
    systemCmd += " -o " + outputFile;

    std::cout << "[ULC] Komut Çalıştırılıyor: " << systemCmd << "\n";

    // Komutu işletim sistemine gönder
    int result = std::system(systemCmd.c_str());

    if (result == 0) {
        std::cout << "[ULC] Başarıyla oluşturuldu: " << outputFile << "\n";
    } else {
        std::cerr << "[HATA] Derleme sırasında bir hata meydana geldi.\n";
    }

    return result;
}