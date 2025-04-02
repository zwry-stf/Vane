#include "../Vane.h"
#include <shlobj_core.h>
#include <fstream>

namespace fs = std::filesystem;

bool Vane::Config::Init(const std::string& projName)
{
    std::lock_guard<std::mutex> lock(mutex);

    char documents[MAX_PATH];
    if (FAILED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, documents)))
        return false;

    config_path = fs::path(documents) / projName / "cfg";

    std::error_code ec;
    if (!fs::exists(config_path, ec) && !fs::create_directories(config_path, ec))
        return false;

    fs::path lastConfigPath = config_path / "last";
    if (!fs::exists(lastConfigPath, ec))
    {
        std::ofstream last(lastConfigPath, std::ios::binary);
        if (last.is_open())
            last << "config_0.cfg";
        last.close();
    }

    // Read last used file
    std::ifstream lastFile(lastConfigPath, std::ios::binary);
    if (lastFile.is_open())
        std::getline(lastFile, curr_file);
    lastFile.close();

    if (!fs::exists(config_path / curr_file))
        curr_file = "config_0.cfg";

    // Create if non existent
    if (!fs::exists(config_path / curr_file))
    {
        std::ofstream file(config_path / curr_file, std::ios::binary);
        if (!file.is_open())
            return false;

        file.close();
    }

    return true;
}

bool Vane::Config::Save()
{
    if (!SaveLastFile())
        return false;

    XyVector<unsigned char> buffer;
    for (const auto& module : module_configs)
    {
        module->Save(buffer);
    }

    std::ofstream file(config_path / curr_file, std::ios::binary);
    if (!file.is_open())
        return false;

    file.write(reinterpret_cast<const char*>(buffer.Data), buffer.size());
    return file.good();
}

bool Vane::Config::Load()
{
    if (!SaveLastFile())
        return false;
    // First, reset all modules to their default values.
    for (auto& module : module_configs)
    {
        module->Reset();
    }

    std::ifstream file(config_path / curr_file, std::ios::binary);
    if (!file.is_open())
        return false;

    // Read the entire file into a buffer.
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    size_t pos = 0;
    while (pos < buffer.size())
    {
        // Ensure there is at least one byte for the name length.
        if (pos + 1 > buffer.size())
            break;
        unsigned char nameLen = buffer[pos];
        pos++;

        // Ensure the buffer contains the full name.
        if (pos + nameLen > buffer.size())
            break;
        std::string storedName(reinterpret_cast<const char*>(&buffer[pos]), nameLen);
        pos += nameLen;

        // Ensure there is a byte for the data size.
        if (pos + 1 > buffer.size())
            break;
        unsigned char dataSize = buffer[pos];
        pos++;

        // Ensure there is enough data.
        if (pos + dataSize > buffer.size())
            break;
        const unsigned char* rawData = &buffer[pos];
        pos += dataSize;

        // Try to load this data into the corresponding module.
        for (auto& module : module_configs)
        {
            // Each module checks if the stored name matches its name.
            if (module->Load(storedName.c_str(), rawData))
                break;
        }
    }
    return true;
}

bool Vane::Config::SaveLastFile()
{
    fs::path lastConfigPath = config_path / "last";
    std::ofstream last(lastConfigPath, std::ios::binary);
    if (!last.is_open())
        return false;

    last << curr_file;
    last.close();
    return true;
}

void Vane::Config::SetCurr()
{
    std::lock_guard<std::mutex> lock(mutex);
    curr_file = configs[selected_config].name;
}

bool Vane::Config::FetchConfigs()
{
    int currentIndex = 0;
    {
        std::lock_guard<std::mutex> lock(mutex);
        configs.clear();
        for (const auto& entry : fs::directory_iterator(config_path))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".cfg")
            {
                configs.emplace_back(entry.path().filename().string());
                if (configs.back().name == curr_file)
                    currentIndex = configs.size() - 1;
            }
        }
    }

    if (configs.empty())
    {
        if (!Create())
        {
            XY_ASSERT(false && "Failed to create config");
            selected_config = -1;
            return true;
        }
    }
    selected_config = currentIndex;
    SetCurr();

    return true;
}

bool Vane::Config::Create()
{
    std::lock_guard<std::mutex> lock(Config::mutex);
    std::string name;
    for (int i = 0;; ++i)
    {
        name = "config_" + std::to_string(i) + ".cfg";
        if (!fs::exists(config_path / name))
            break;

        // Why would you need this many default named configs??
        if (i > 30)
        {
            return false;
        }
    }

    std::ofstream file(config_path / name, std::ios::binary);
    if (!file.is_open())
        return false;

    configs.push_back(ConfigDrawable(name));
    return true;
}

bool Vane::Config::Remove()
{
    bool success = false;
    {
        std::lock_guard<std::mutex> lock(Config::mutex);
        std::error_code ec;
        success = fs::remove(config_path / curr_file, ec);

        configs.erase(configs.begin() + selected_config);

        if (selected_config >= static_cast<int>(configs.size()))
            selected_config = configs.size() - 1;
    }

    if (configs.empty())
    {
        Create();
        selected_config = 0;
    }

    SetCurr();

    return success;
}

bool Vane::Config::Rename(const std::string& new_name)
{
    std::error_code ec;
    {
        std::lock_guard<std::mutex> lock(Config::mutex);
        fs::rename(config_path / curr_file, config_path / new_name, ec);

        configs[selected_config].name = new_name;
    }

    SetCurr();

    return !ec;
}
