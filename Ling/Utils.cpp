#include "Precompiled.hpp"
#include "Utils.hpp"
#include <fstream>
#include "Player.hpp"
using namespace std;
using namespace json_spirit;
using boost::filesystem::path;
void GetMediaFiles(const char* p_media_repo, std::vector<path>& p_media_files)
{
    if(!p_media_repo){ 
        return;
    }
    path p_media_dir (p_media_repo);
    typedef boost::filesystem::directory_iterator DI;
    typedef boost::filesystem::recursive_directory_iterator RDI;

    if( !boost::filesystem::is_directory(p_media_dir)){
        LOG(logERROR) << "Can't find the media repository: " << p_media_dir.string();
        return ;
    }

    Player player(NULL);
    RDI beg(p_media_dir), end;
    while(beg != end){
        if (!is_directory(*beg) && !boost::filesystem::is_empty(*beg)){
            if(player.IsSupport(beg->path().wstring())){
                p_media_files.push_back(beg->path().wstring());
            }else{
                LOG(logWARNING) << "Can't recogonize file format:" << beg->path().string();
            }
        }
        beg++;
    }
    return;
}


FileInfo LingJson::ReadHistory(const Object& obj)
{
    FileInfo fh;
    for( Object::size_type i = 0; i != obj.size(); ++i )
    {
        const Pair& pair = obj[i];

        const string& name  = pair.name_;
        const Value&  value = pair.value_;

        if( name == "file")
        {
            //convert the string to wstring
            fh.file_ = boost::filesystem::path(value.get_str()).wstring();
        }else if(name == "sections")
        {
             const Array& sec_array = value.get_array();
             for(unsigned int j = 0; j < sec_array.size(); ++j)
             {
                 const Object& sec_obj = sec_array[j].get_obj();                 
                 Section sec;
                 for(unsigned int k = 0; k < sec_obj.size(); ++k)
                 {
                     const Pair& p = sec_obj[k];
                     if(p.name_ == "begin")
                          sec.begin_ = p.value_.get_int();
                     if(p.name_ == "end")
                         sec.end_ = p.value_.get_int();
                 }
                 fh.sections_.push_back(sec);
             }
        }
    }
    return fh;
}
LingJson::LingJson()
{
    char szFilePath[MAX_PATH + 1];
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    json_path_ = szFilePath;
    json_path_.replace_extension(".json");

    char* p_home = std::getenv("HOME");
    if( p_home != NULL){
        boost::filesystem::path home_path(p_home);
        if( boost::filesystem::is_directory(home_path)) {
            if(boost::filesystem::is_regular_file(home_path / "ling.json")){
                json_path_ = home_path / "ling.json";
            }
        }
    }
    LOG(logINFO) << "Getting the configuraion info from " << json_path_.string();
    if( !boost::filesystem::is_regular_file(json_path_)) 
    {
        LOG(logWARNING) << json_path_.string() << " does not exist";
        return;
    }

    std::ifstream json_stream(json_path_.string().c_str());
    try{
        Value value;
        read_stream(json_stream, value);
        const Object& obj = value.get_obj();

        for( Object::size_type i = 0; i != obj.size(); ++i ){
            const Pair& pair = obj[i];

            const string& name  = pair.name_;
            const Value&  value = pair.value_;
            if( name == "repo"){
                repo_ = value.get_str();
            }
            if( name == "hotkey"){
                Hotkey hk;
                const Array& arr = value.get_array();
                for(unsigned int i = 0; i < arr.size(); ++i){
                    const Array& arr2 = arr[i].get_array();
                    hk.vk_ = arr2[0].get_int();
                    hk.fsModifiers_ = arr2[1].get_int();
                    hotkeys_.push_back(hk);
                }
            }                                       
            if( name == "history"){
                const Array& arr = value.get_array();
                for(unsigned int i = 0; i < arr.size(); ++i){
                    file_infos_.push_back(ReadHistory(arr[i].get_obj()));
                }
            }
        }
    }
    catch (std::exception& e){
        LOG(logERROR) << "std exception occured";
    }

}

LingJson::~LingJson()
{
    Object root_object;

    Array history_array;

    for(FileInfos::const_iterator ci = file_infos_.begin(); ci != file_infos_.end(); ++ci)
    {
        Object file_info_object;

        Array sections_array;
        for(Sections::const_iterator sci = ci->sections_.begin();  sci != ci->sections_.end(); ++sci)
        {
             Object section_object;
             section_object.push_back(Pair("begin", sci->begin_));
             section_object.push_back(Pair("end", sci->end_));
             sections_array.push_back(section_object);
        }
        file_info_object.push_back(Pair("file", boost::filesystem::path(ci->file_).string()));
        file_info_object.push_back(Pair("sections", sections_array));
        history_array.push_back(file_info_object);
    }

    Array hotkey_array;
    for(std::vector<Hotkey>::const_iterator vhci = hotkeys_.begin(); vhci != hotkeys_.end(); ++vhci){
        std::vector< int > int_vect;
        int_vect.push_back( vhci->vk_);
        int_vect.push_back( vhci->fsModifiers_);
        const Value val( int_vect.begin(), int_vect.end() );
        hotkey_array.push_back(val);
    }
    root_object.push_back(Pair("repo", repo_));
    root_object.push_back(Pair("hotkey", hotkey_array));
    root_object.push_back(Pair("history", history_array));

    ofstream os( json_path_.string().c_str());
    write_stream( Value( root_object ), os, pretty_print );

}
FileInfo LingJson::GetFileInfo(std::wstring file_name)const
{
    for(FileInfos::const_iterator ci = file_infos_.begin(); ci != file_infos_.end(); ++ci)
    {
        if( ci->file_ ==  file_name ){
            return *ci;
        }
    }
    FileInfo fi;
    fi.file_ = file_name;
    return fi;
}

void LingJson::UpdateFileInfo(const FileInfo& file_info)
{
    for(FileInfos::iterator ci = file_infos_.begin(); ci != file_infos_.end(); ++ci)
    {
        if( ci->file_ == file_info.file_ ){
            *ci = file_info;
            return;
        }
    }
    file_infos_.push_back(file_info);
}

const char* LingJson::GetRepoPath()const
{
    return repo_.c_str();
}
std::ostream& operator<<(std::ostream& os, const LingJson& json)
{
    os << "repo = " << json.repo_ ;
    for(unsigned int i = 0 ; i <  json.file_infos_.size(); ++i)
    {
        os << " file = " << path(json.file_infos_[i].file_).string() ;
        for (unsigned int j = 0; j < json.file_infos_[i].sections_.size(); ++j)
        {
            os  << " (" << json.file_infos_[i].sections_[j].begin_ 
                << ", " << json.file_infos_[i].sections_[j].end_ << ")"; 
        }
    }
    return os;
}
std::wstring GetTime(long ms){
    long secs = ms /1000;
    long minute = secs / 60;
    long sec = secs % 60;
    wchar_t buffer[32];
    swprintf(buffer, 32, L"%d:%d", minute, sec);
    return buffer;
}
        
