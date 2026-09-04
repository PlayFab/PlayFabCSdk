struct DeviceGameSaveState;

#include <filesystem>
#include <string>

HRESULT ResolveSaveRoot(DeviceGameSaveState* state, std::filesystem::path& rootPath, std::string& error);
HRESULT ConvertFilesystemError(const std::error_code& ec);

