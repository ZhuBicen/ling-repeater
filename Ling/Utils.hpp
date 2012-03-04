#ifndef LING_UTILS_HPP
#define LING_UTILS_HPP
using boost::filesystem::path;


struct Section
{
    long begin_;
    long end_;
};
typedef std::vector<Section> Sections;
struct FileInfo{
    std::wstring file_;
    Sections sections_;
};


class LingJson
{
public:
    LingJson();
    ~LingJson();
    FileInfo GetFileInfo(std::wstring file_name)const ;
    void UpdateFileInfo(const FileInfo& file_info);
    const char* GetRepoPath()const ;

    friend std::ostream& operator<<(std::ostream& os, const LingJson&);

private:

    typedef std::vector<FileInfo> FileInfos;
    FileInfos file_infos_;
    std::string repo_;
    boost::filesystem::path json_path_;
private:
    FileInfo LingJson::ReadHistory(const json_spirit::Object& obj);
};

void GetMediaFiles(const char* p_media_repo, std::vector<path>& p_media_files);
std::wstring GetTime(long ms);
#endif
