#include <iostream>
#include <fstream>
#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/pwdbased.h>
using namespace CryptoPP;

SecByteBlock DeriveKey(const std::string& password)
{
    SecByteBlock derived(AES::DEFAULT_KEYLENGTH);
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
    const byte* salt = (const byte*)"somesalt"; 
    size_t saltLen = 8; 
    pbkdf.DeriveKey(derived, derived.size(), 0, (const
        byte*)password.data(), password.size(), salt, saltLen, 1000, 0.0);
    return derived;
}

void ProcessFile(const std::string& inputFile, const std::string& outputFile, const std::string& password, bool encrypt)
{
    try {
        std::ifstream in(inputFile, std::ios::binary);
        std::ofstream out(outputFile, std::ios::binary);
        if (!in.is_open() || !out.is_open()) {
            std::cout << "Ошибка открытия файлов!" << std::endl;
            return;
        }
        SecByteBlock key = DeriveKey(password);
        byte iv[AES::BLOCKSIZE];
        memset(iv, 0x00, AES::BLOCKSIZE);
        if (encrypt) {
            CBC_Mode<AES>::Encryption enc;
            enc.SetKeyWithIV(key, key.size(), iv);
            FileSource fileSrc(in, true, new StreamTransformationFilter(enc, new FileSink(out)));
            fileSrc.PumpAll();
            fileSrc.Flush(true);
            std::cout << "Файл зашифрован!" << std::endl;
        }
        else {
            CBC_Mode<AES>::Decryption dec;
            dec.SetKeyWithIV(key, key.size(), iv);
            FileSource fileSrc(in, true, new StreamTransformationFilter(dec, new FileSink(out)));
            fileSrc.PumpAll();
            fileSrc.Flush(true);
            std::cout << "Файл расшифрован!" << std::endl;
        }
    }
    catch (const Exception& ex) {
        std::cerr << "Crypto++ исключение: " << ex.what() <<
            std::endl;
    }
}
int main()
{
    std::string inputFile, outputFile, password;
    int choice;
    std::cout << "Выберите режим работы:\n1. Зашифровать файл\n2.Расшифровать файл\n";
    std::cin >> choice;
    std::cout << "Введите имя входного файла: ";
    std::cin >> inputFile;
    std::cout << "Введите имя выходного файла: ";
    std::cin >> outputFile;
    std::cout << "Введите пароль: ";
    std::cin >> password;
    bool encrypt = (choice == 1);
    ProcessFile(inputFile, outputFile, password, encrypt);
    return 0;
}
