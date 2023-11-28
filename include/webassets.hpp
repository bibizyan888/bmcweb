#pragma once

#include "../http/app.hpp"

#include <boost/algorithm/string/predicate.hpp>

#include <cstring>
#include <filesystem>

namespace crow
{

namespace webassets
{
struct CmpStr
{
    bool operator()(const char* a, const char* b)
    {
        return std::strcmp(a, b) < 0;
    }
};

inline void requestRoutes(App& app)
{
    constexpr static std::array<std::pair<const char*, const char*>, 17>
        contentTypes{
            {{".css", "text/css;charset=UTF-8"},
             {".html", "text/html;charset=UTF-8"},
             {".js", "application/javascript;charset=UTF-8"},
             {".png", "image/png;charset=UTF-8"},
             {".woff", "application/x-font-woff"},
             {".woff2", "application/x-font-woff2"},
             {".gif", "image/gif"},
             {".ico", "image/x-icon"},
             {".ttf", "application/x-font-ttf"},
             {".svg", "image/svg+xml"},
             {".eot", "application/vnd.ms-fontobject"},
             {".xml", "application/xml"},
             {".json", "application/json"},
             {".jpg", "image/jpeg"},
             {".jpeg", "image/jpeg"},
             {".json", "application/json"},
             // dev tools don't care about map type, setting to json causes
             // browser to show as text
             // https://stackoverflow.com/questions/19911929/what-mime-type-should-i-use-for-javascript-source-map-files
             {".map", "application/json"}}};

    std::filesystem::path rootpath{"/home/np/acad/bmcweb/www"};
    std::filesystem::recursive_directory_iterator dirIter(rootpath);

    std::vector<std::filesystem::directory_entry> paths(
        std::filesystem::begin(dirIter), std::filesystem::end(dirIter));

    std::sort(paths.rbegin(), paths.rend());

    for (const std::filesystem::directory_entry& dir : paths)
    {
        const std::filesystem::path& absolutePath = dir.path();
        std::filesystem::path relativePath{
            absolutePath.string().substr(rootpath.string().size() - 1)};
        if (std::filesystem::is_directory(dir))
        {
            if (boost::starts_with(dir.path().filename().string(), ".") ||
                std::filesystem::is_symlink(dir))
            {
                dirIter.disable_recursion_pending();
            }
        }
        else if (std::filesystem::is_regular_file(dir))
        {
            std::string extension = relativePath.extension();
            std::filesystem::path webpath = relativePath;
            const char* contentEncoding = nullptr;

            if (extension == ".gz")
            {
                webpath = webpath.replace_extension("");
                extension = webpath.extension().string();
                contentEncoding = "gzip";
            }

            if (boost::starts_with(webpath.filename().string(), "index."))
            {
                webpath = webpath.parent_path();
                if (webpath.string().size() == 0 ||
                    webpath.string().back() != '/')
                {
                    // insert into webroutes
                    webpath += '/';
                }
            }
        }
    }
}

} // namespace webassets

} // namespace crow
